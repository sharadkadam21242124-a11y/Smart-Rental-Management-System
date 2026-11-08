#include "Booking.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Booking::Booking()
    : m_id(0), m_bookingCode(""), m_customerId(0), m_itemId(0),
      m_startDate(""), m_endDate(""), m_rentalDays(1), m_dailyRate(0.0),
      m_baseRent(0.0), m_discountRate(0.0), m_discountAmount(0.0),
      m_securityDeposit(0.0), m_totalAmount(0.0), m_bookingStatus("pending"),
      m_createdAt(""), m_updatedAt(""), m_itemTitle(""), m_customerName(""), m_category("") {}

Booking::Booking(int id, const std::string& code, int customerId, int itemId,
                 const std::string& startDate, const std::string& endDate,
                 int rentalDays, double dailyRate, double baseRent,
                 double discountRate, double discountAmount, double securityDeposit,
                 double totalAmount, const std::string& status,
                 const std::string& createdAt, const std::string& updatedAt)
    : m_id(id), m_bookingCode(code), m_customerId(customerId), m_itemId(itemId),
      m_startDate(startDate), m_endDate(endDate), m_rentalDays(rentalDays),
      m_dailyRate(dailyRate), m_baseRent(baseRent), m_discountRate(discountRate),
      m_discountAmount(discountAmount), m_securityDeposit(securityDeposit),
      m_totalAmount(totalAmount), m_bookingStatus(status),
      m_createdAt(createdAt), m_updatedAt(updatedAt),
      m_itemTitle(""), m_customerName(""), m_category("") {
    if (rentalDays < 1) throw ValidationException("Rental duration must be at least 1 day.");
    if (startDate >= endDate) throw ValidationException("Start date must be strictly prior to end date.");
}

int Booking::getId() const { return m_id; }
std::string Booking::getBookingCode() const { return m_bookingCode; }
int Booking::getCustomerId() const { return m_customerId; }
int Booking::getItemId() const { return m_itemId; }
std::string Booking::getStartDate() const { return m_startDate; }
std::string Booking::getEndDate() const { return m_endDate; }
int Booking::getRentalDays() const { return m_rentalDays; }
double Booking::getDailyRate() const { return m_dailyRate; }
double Booking::getBaseRent() const { return m_baseRent; }
double Booking::getDiscountRate() const { return m_discountRate; }
double Booking::getDiscountAmount() const { return m_discountAmount; }
double Booking::getSecurityDeposit() const { return m_securityDeposit; }
double Booking::getTotalAmount() const { return m_totalAmount; }
std::string Booking::getBookingStatus() const { return m_bookingStatus; }
std::string Booking::getCreatedAt() const { return m_createdAt; }
std::string Booking::getUpdatedAt() const { return m_updatedAt; }

std::string Booking::getItemTitle() const { return m_itemTitle; }
std::string Booking::getCustomerName() const { return m_customerName; }
std::string Booking::getCategory() const { return m_category; }

void Booking::setId(int id) { m_id = id; }
void Booking::setBookingCode(const std::string& code) { m_bookingCode = code; }
void Booking::setCustomerId(int id) { m_customerId = id; }
void Booking::setItemId(int id) { m_itemId = id; }
void Booking::setStartDate(const std::string& start) { m_startDate = start; }
void Booking::setEndDate(const std::string& end) { m_endDate = end; }
void Booking::setRentalDays(int days) { m_rentalDays = days; }
void Booking::setDailyRate(double rate) { m_dailyRate = rate; }
void Booking::setBaseRent(double rent) { m_baseRent = rent; }
void Booking::setDiscountRate(double rate) { m_discountRate = rate; }
void Booking::setDiscountAmount(double amount) { m_discountAmount = amount; }
void Booking::setSecurityDeposit(double deposit) { m_securityDeposit = deposit; }
void Booking::setTotalAmount(double total) { m_totalAmount = total; }
void Booking::setBookingStatus(const std::string& status) { m_bookingStatus = status; }
void Booking::setCreatedAt(const std::string& created) { m_createdAt = created; }
void Booking::setUpdatedAt(const std::string& updated) { m_updatedAt = updated; }

void Booking::setItemTitle(const std::string& title) { m_itemTitle = title; }
void Booking::setCustomerName(const std::string& name) { m_customerName = name; }
void Booking::setCategory(const std::string& cat) { m_category = cat; }

bool Booking::operator<(const Booking& other) const {
    return this->m_totalAmount < other.m_totalAmount;
}

bool Booking::operator==(const Booking& other) const {
    return this->m_bookingCode == other.m_bookingCode;
}

std::string Booking::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("booking_code", m_bookingCode)
       << JsonHelper::kv("customer_id", m_customerId)
       << JsonHelper::kv("item_id", m_itemId)
       << JsonHelper::kvString("item_title", m_itemTitle)
       << JsonHelper::kvString("category", m_category)
       << JsonHelper::kvString("customer_name", m_customerName)
       << JsonHelper::kvString("start_date", m_startDate)
       << JsonHelper::kvString("end_date", m_endDate)
       << JsonHelper::kv("rental_days", m_rentalDays)
       << JsonHelper::kv("daily_rate", m_dailyRate)
       << JsonHelper::kv("base_rent", m_baseRent)
       << JsonHelper::kv("discount_rate", m_discountRate)
       << JsonHelper::kv("discount_amount", m_discountAmount)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kv("total_amount", m_totalAmount)
       << JsonHelper::kvString("booking_status", m_bookingStatus)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
