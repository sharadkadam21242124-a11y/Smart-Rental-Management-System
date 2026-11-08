#include "BookingRepository.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"
#include <sstream>
#include <chrono>
#include <random>
#include <cmath>

BookingRepository::BookingRepository(Database& db) : m_db(db) {}

bool BookingRepository::isItemAvailableForDates(int itemId, const std::string& startDate,
                                               const std::string& endDate, int excludeBookingId) {
    std::ostringstream ss;
    ss << "SELECT COUNT(*) AS conflict_count FROM bookings "
       << "WHERE item_id = " << itemId << " "
       << "AND booking_status IN ('confirmed', 'active') "
       << "AND NOT (end_date < '" << m_db.escapeString(startDate) << "' "
       << "OR start_date > '" << m_db.escapeString(endDate) << "')";

    if (excludeBookingId > 0) {
        ss << " AND id != " << excludeBookingId;
    }
    ss << ";";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return true;
    return res[0].getInt("conflict_count") == 0;
}

std::shared_ptr<Booking> BookingRepository::createBooking(int customerId, int itemId,
                                                        const std::string& startDate,
                                                        const std::string& endDate) {
    if (startDate >= endDate) {
        throw ValidationException("Booking start date must be before end date.");
    }

    // Verify item exists
    std::ostringstream ssItem;
    ssItem << "SELECT id, title, category, daily_rate, security_deposit, status "
           << "FROM rental_items WHERE id = " << itemId << " LIMIT 1;";
    QueryResult itemRes = m_db.executeQuery(ssItem.str());
    if (itemRes.empty()) {
        throw NotFoundException("Rental item not found with ID: " + std::to_string(itemId));
    }

    const auto& itemRow = itemRes[0];
    if (itemRow.getString("status") == "maintenance" || itemRow.getString("status") == "retired") {
        throw ValidationException("Rental item is currently under " + itemRow.getString("status") + " and cannot be booked.");
    }

    // Overlapping booking prevention
    if (!isItemAvailableForDates(itemId, startDate, endDate)) {
        throw BookingConflictException("The selected item is already booked for these dates. Please choose different dates.");
    }

    // Calculate rental days using MySQL DATEDIFF
    std::ostringstream ssDiff;
    ssDiff << "SELECT DATEDIFF('" << m_db.escapeString(endDate) << "', '"
           << m_db.escapeString(startDate) << "') AS days;";
    QueryResult diffRes = m_db.executeQuery(ssDiff.str());
    int days = diffRes.empty() ? 1 : diffRes[0].getInt("days");
    if (days < 1) days = 1;

    double dailyRate = itemRow.getDouble("daily_rate");
    double securityDeposit = itemRow.getDouble("security_deposit");
    double baseRent = days * dailyRate;

    // Apply Tiered Duration Discount
    double discountRate = 0.0;
    if (days >= 30) discountRate = 20.0;
    else if (days >= 14) discountRate = 15.0;
    else if (days >= 7) discountRate = 10.0;
    else if (days >= 5) discountRate = 5.0;

    double discountAmount = baseRent * (discountRate / 100.0);
    double totalAmount = (baseRent - discountAmount) + securityDeposit;

    // Generate unique booking code
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, 999);
    std::ostringstream ssCode;
    ssCode << "BK-2026-" << (now % 100000) << dis(gen);
    std::string bookingCode = ssCode.str();

    TransactionGuard guard(m_db);

    std::ostringstream ssIns;
    ssIns << "INSERT INTO bookings (booking_code, customer_id, item_id, start_date, end_date, rental_days, "
          << "daily_rate, base_rent, discount_rate, discount_amount, security_deposit, total_amount, booking_status) "
          << "VALUES ('" << bookingCode << "', " << customerId << ", " << itemId << ", '"
          << m_db.escapeString(startDate) << "', '" << m_db.escapeString(endDate) << "', " << days << ", "
          << dailyRate << ", " << baseRent << ", " << discountRate << ", " << discountAmount << ", "
          << securityDeposit << ", " << totalAmount << ", 'confirmed');";
    m_db.executeUpdate(ssIns.str());
    int bookingId = static_cast<int>(m_db.getLastInsertId());

    // Update rental item status to rented
    std::ostringstream ssUpd;
    ssUpd << "UPDATE rental_items SET status = 'rented' WHERE id = " << itemId << ";";
    m_db.executeUpdate(ssUpd.str());

    guard.commit();

    auto booking = std::make_shared<Booking>(
        bookingId, bookingCode, customerId, itemId, startDate, endDate, days,
        dailyRate, baseRent, discountRate, discountAmount, securityDeposit,
        totalAmount, "confirmed"
    );
    booking->setItemTitle(itemRow.getString("title"));
    booking->setCategory(itemRow.getString("category"));
    return booking;
}

std::shared_ptr<Booking> BookingRepository::findById(int id) {
    std::ostringstream ss;
    ss << "SELECT b.*, i.title AS item_title, i.category, c.full_name AS customer_name "
       << "FROM bookings b "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "JOIN customers c ON b.customer_id = c.id "
       << "WHERE b.id = " << id << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto b = std::make_shared<Booking>(
        r.getInt("id"), r.getString("booking_code"), r.getInt("customer_id"),
        r.getInt("item_id"), r.getString("start_date"), r.getString("end_date"),
        r.getInt("rental_days"), r.getDouble("daily_rate"), r.getDouble("base_rent"),
        r.getDouble("discount_rate"), r.getDouble("discount_amount"),
        r.getDouble("security_deposit"), r.getDouble("total_amount"),
        r.getString("booking_status"), r.getString("created_at"), r.getString("updated_at")
    );
    b->setItemTitle(r.getString("item_title"));
    b->setCustomerName(r.getString("customer_name"));
    b->setCategory(r.getString("category"));
    return b;
}

std::shared_ptr<Booking> BookingRepository::findByCode(const std::string& code) {
    std::ostringstream ss;
    ss << "SELECT b.*, i.title AS item_title, i.category, c.full_name AS customer_name "
       << "FROM bookings b "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "JOIN customers c ON b.customer_id = c.id "
       << "WHERE b.booking_code = '" << m_db.escapeString(code) << "' LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto b = std::make_shared<Booking>(
        r.getInt("id"), r.getString("booking_code"), r.getInt("customer_id"),
        r.getInt("item_id"), r.getString("start_date"), r.getString("end_date"),
        r.getInt("rental_days"), r.getDouble("daily_rate"), r.getDouble("base_rent"),
        r.getDouble("discount_rate"), r.getDouble("discount_amount"),
        r.getDouble("security_deposit"), r.getDouble("total_amount"),
        r.getString("booking_status"), r.getString("created_at"), r.getString("updated_at")
    );
    b->setItemTitle(r.getString("item_title"));
    b->setCustomerName(r.getString("customer_name"));
    b->setCategory(r.getString("category"));
    return b;
}

std::vector<std::shared_ptr<Booking>> BookingRepository::findByCustomerId(int customerId) {
    std::vector<std::shared_ptr<Booking>> list;
    std::ostringstream ss;
    ss << "SELECT b.*, i.title AS item_title, i.category, c.full_name AS customer_name "
       << "FROM bookings b "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "JOIN customers c ON b.customer_id = c.id "
       << "WHERE b.customer_id = " << customerId << " "
       << "ORDER BY b.id DESC;";

    QueryResult res = m_db.executeQuery(ss.str());
    for (const auto& r : res.getRows()) {
        auto b = std::make_shared<Booking>(
            r.getInt("id"), r.getString("booking_code"), r.getInt("customer_id"),
            r.getInt("item_id"), r.getString("start_date"), r.getString("end_date"),
            r.getInt("rental_days"), r.getDouble("daily_rate"), r.getDouble("base_rent"),
            r.getDouble("discount_rate"), r.getDouble("discount_amount"),
            r.getDouble("security_deposit"), r.getDouble("total_amount"),
            r.getString("booking_status"), r.getString("created_at"), r.getString("updated_at")
        );
        b->setItemTitle(r.getString("item_title"));
        b->setCustomerName(r.getString("customer_name"));
        b->setCategory(r.getString("category"));
        list.push_back(b);
    }
    return list;
}

std::vector<std::shared_ptr<Booking>> BookingRepository::findAllBookings(const std::string& status) {
    std::vector<std::shared_ptr<Booking>> list;
    std::ostringstream ss;
    ss << "SELECT b.*, i.title AS item_title, i.category, c.full_name AS customer_name "
       << "FROM bookings b "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "JOIN customers c ON b.customer_id = c.id ";

    if (!status.empty()) {
        ss << "WHERE b.booking_status = '" << m_db.escapeString(status) << "' ";
    }
    ss << "ORDER BY b.id DESC;";

    QueryResult res = m_db.executeQuery(ss.str());
    for (const auto& r : res.getRows()) {
        auto b = std::make_shared<Booking>(
            r.getInt("id"), r.getString("booking_code"), r.getInt("customer_id"),
            r.getInt("item_id"), r.getString("start_date"), r.getString("end_date"),
            r.getInt("rental_days"), r.getDouble("daily_rate"), r.getDouble("base_rent"),
            r.getDouble("discount_rate"), r.getDouble("discount_amount"),
            r.getDouble("security_deposit"), r.getDouble("total_amount"),
            r.getString("booking_status"), r.getString("created_at"), r.getString("updated_at")
        );
        b->setItemTitle(r.getString("item_title"));
        b->setCustomerName(r.getString("customer_name"));
        b->setCategory(r.getString("category"));
        list.push_back(b);
    }
    return list;
}

std::vector<std::shared_ptr<Booking>> BookingRepository::findActiveRentals() {
    return findAllBookings("active");
}

bool BookingRepository::cancelBooking(int bookingId, int customerId) {
    auto b = findById(bookingId);
    if (!b) throw NotFoundException("Booking not found: " + std::to_string(bookingId));
    if (customerId > 0 && b->getCustomerId() != customerId) {
        throw ValidationException("You are not authorized to cancel this booking.");
    }
    if (b->getBookingStatus() == "completed" || b->getBookingStatus() == "cancelled") {
        throw ValidationException("Booking is already " + b->getBookingStatus());
    }

    TransactionGuard guard(m_db);

    std::ostringstream ss;
    ss << "UPDATE bookings SET booking_status = 'cancelled' WHERE id = " << bookingId << ";";
    m_db.executeUpdate(ss.str());

    // Release rental item back to available
    std::ostringstream ssItem;
    ssItem << "UPDATE rental_items SET status = 'available' WHERE id = " << b->getItemId() << ";";
    m_db.executeUpdate(ssItem.str());

    guard.commit();
    return true;
}

bool BookingRepository::confirmBooking(int bookingId) {
    std::ostringstream ss;
    ss << "UPDATE bookings SET booking_status = 'active' WHERE id = " << bookingId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool BookingRepository::updateStatus(int bookingId, const std::string& status) {
    std::ostringstream ss;
    ss << "UPDATE bookings SET booking_status = '" << m_db.escapeString(status)
       << "' WHERE id = " << bookingId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

int BookingRepository::getTotalBookingCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM bookings;");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}

int BookingRepository::getActiveBookingCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM bookings WHERE booking_status IN ('confirmed', 'active');");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}

int BookingRepository::getPendingBookingCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM bookings WHERE booking_status = 'pending';");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}
