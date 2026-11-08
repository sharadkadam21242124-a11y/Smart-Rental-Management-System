#include "PaymentRepository.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"
#include <sstream>
#include <algorithm>

PaymentRepository::PaymentRepository(Database& db) : m_db(db) {}

std::shared_ptr<Payment> PaymentRepository::recordPayment(
    int bookingId,
    int customerId,
    double amount,
    const std::string& method,
    const std::string& type,
    const std::string& remarks
) {
    if (amount <= 0.0) throw ValidationException("Payment amount must be greater than zero.");

    std::string txnId = Payment::generateTransactionId(method);

    TransactionGuard guard(m_db);

    std::ostringstream ss;
    ss << "INSERT INTO payments (transaction_id, booking_id, customer_id, amount, payment_method, payment_type, payment_status, remarks) "
       << "VALUES ('" << txnId << "', " << bookingId << ", " << customerId << ", " << amount << ", '"
       << m_db.escapeString(method) << "', '" << m_db.escapeString(type) << "', 'success', '"
       << m_db.escapeString(remarks) << "');";
    m_db.executeUpdate(ss.str());
    int payId = static_cast<int>(m_db.getLastInsertId());

    // Update booking status to active/confirmed if pending
    std::ostringstream ssUpd;
    ssUpd << "UPDATE bookings SET booking_status = 'active' WHERE id = " << bookingId
          << " AND booking_status IN ('pending', 'confirmed');";
    m_db.executeUpdate(ssUpd.str());

    // Update invoice paid amount
    std::ostringstream ssInv;
    ssInv << "UPDATE invoices SET paid_amount = paid_amount + " << amount
          << ", payment_status = CASE WHEN (paid_amount + " << amount << ") >= total_amount THEN 'paid' ELSE 'partial' END "
          << "WHERE booking_id = " << bookingId << ";";
    m_db.executeUpdate(ssInv.str());

    guard.commit();

    return std::make_shared<Payment>(
        payId, txnId, bookingId, customerId, amount, method, type, "success", "", remarks
    );
}

std::shared_ptr<Payment> PaymentRepository::findById(int id) {
    std::ostringstream ss;
    ss << "SELECT p.*, b.booking_code, c.full_name AS customer_name "
       << "FROM payments p "
       << "JOIN bookings b ON p.booking_id = b.id "
       << "JOIN customers c ON p.customer_id = c.id "
       << "WHERE p.id = " << id << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto p = std::make_shared<Payment>(
        r.getInt("id"), r.getString("transaction_id"), r.getInt("booking_id"),
        r.getInt("customer_id"), r.getDouble("amount"), r.getString("payment_method"),
        r.getString("payment_type"), r.getString("payment_status"),
        r.getString("payment_date"), r.getString("remarks")
    );
    p->setBookingCode(r.getString("booking_code"));
    p->setCustomerName(r.getString("customer_name"));
    return p;
}

std::shared_ptr<Payment> PaymentRepository::findByTransactionId(const std::string& txnId) {
    std::ostringstream ss;
    ss << "SELECT p.*, b.booking_code, c.full_name AS customer_name "
       << "FROM payments p "
       << "JOIN bookings b ON p.booking_id = b.id "
       << "JOIN customers c ON p.customer_id = c.id "
       << "WHERE p.transaction_id = '" << m_db.escapeString(txnId) << "' LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto p = std::make_shared<Payment>(
        r.getInt("id"), r.getString("transaction_id"), r.getInt("booking_id"),
        r.getInt("customer_id"), r.getDouble("amount"), r.getString("payment_method"),
        r.getString("payment_type"), r.getString("payment_status"),
        r.getString("payment_date"), r.getString("remarks")
    );
    p->setBookingCode(r.getString("booking_code"));
    p->setCustomerName(r.getString("customer_name"));
    return p;
}

std::vector<std::shared_ptr<Payment>> PaymentRepository::findByCustomerId(int customerId) {
    std::vector<std::shared_ptr<Payment>> list;
    std::ostringstream ss;
    ss << "SELECT p.*, b.booking_code, c.full_name AS customer_name "
       << "FROM payments p "
       << "JOIN bookings b ON p.booking_id = b.id "
       << "JOIN customers c ON p.customer_id = c.id "
       << "WHERE p.customer_id = " << customerId << " "
       << "ORDER BY p.id DESC;";

    QueryResult res = m_db.executeQuery(ss.str());
    for (const auto& r : res.getRows()) {
        auto p = std::make_shared<Payment>(
            r.getInt("id"), r.getString("transaction_id"), r.getInt("booking_id"),
            r.getInt("customer_id"), r.getDouble("amount"), r.getString("payment_method"),
            r.getString("payment_type"), r.getString("payment_status"),
            r.getString("payment_date"), r.getString("remarks")
        );
        p->setBookingCode(r.getString("booking_code"));
        p->setCustomerName(r.getString("customer_name"));
        list.push_back(p);
    }
    return list;
}

std::vector<std::shared_ptr<Payment>> PaymentRepository::findAllPayments() {
    std::vector<std::shared_ptr<Payment>> list;
    std::string sql = "SELECT p.*, b.booking_code, c.full_name AS customer_name "
                      "FROM payments p "
                      "JOIN bookings b ON p.booking_id = b.id "
                      "JOIN customers c ON p.customer_id = c.id "
                      "ORDER BY p.id DESC;";

    QueryResult res = m_db.executeQuery(sql);
    for (const auto& r : res.getRows()) {
        auto p = std::make_shared<Payment>(
            r.getInt("id"), r.getString("transaction_id"), r.getInt("booking_id"),
            r.getInt("customer_id"), r.getDouble("amount"), r.getString("payment_method"),
            r.getString("payment_type"), r.getString("payment_status"),
            r.getString("payment_date"), r.getString("remarks")
        );
        p->setBookingCode(r.getString("booking_code"));
        p->setCustomerName(r.getString("customer_name"));
        list.push_back(p);
    }
    return list;
}

std::shared_ptr<Invoice> PaymentRepository::createOrGetInvoice(int bookingId) {
    auto inv = findInvoiceByBookingId(bookingId);
    if (inv) return inv;

    std::ostringstream ssBk;
    ssBk << "SELECT b.*, c.full_name AS customer_name, i.title AS item_title "
         << "FROM bookings b "
         << "JOIN customers c ON b.customer_id = c.id "
         << "JOIN rental_items i ON b.item_id = i.id "
         << "WHERE b.id = " << bookingId << " LIMIT 1;";
    QueryResult bkRes = m_db.executeQuery(ssBk.str());
    if (bkRes.empty()) throw NotFoundException("Booking not found: " + std::to_string(bookingId));

    const auto& b = bkRes[0];
    std::string invNum = Invoice::generateInvoiceNumber();
    double subtotal = b.getDouble("base_rent");
    double discount = b.getDouble("discount_amount");
    double deposit = b.getDouble("security_deposit");
    double total = b.getDouble("total_amount");

    std::ostringstream ssIns;
    ssIns << "INSERT INTO invoices (invoice_number, booking_id, issue_date, due_date, subtotal, discount_amount, tax_amount, security_deposit, total_amount, paid_amount, payment_status) "
          << "VALUES ('" << invNum << "', " << bookingId << ", '" << b.getString("start_date") << "', '"
          << b.getString("start_date") << "', " << subtotal << ", " << discount << ", 0.00, "
          << deposit << ", " << total << ", 0.00, 'unpaid');";
    m_db.executeUpdate(ssIns.str());
    int invId = static_cast<int>(m_db.getLastInsertId());

    auto invoice = std::make_shared<Invoice>(
        invId, invNum, bookingId, b.getString("start_date"), b.getString("start_date"),
        subtotal, discount, 0.0, deposit, total, 0.0, "unpaid"
    );
    invoice->setBookingCode(b.getString("booking_code"));
    invoice->setCustomerName(b.getString("customer_name"));
    invoice->setItemTitle(b.getString("item_title"));
    return invoice;
}

std::shared_ptr<Invoice> PaymentRepository::findInvoiceByBookingId(int bookingId) {
    std::ostringstream ss;
    ss << "SELECT inv.*, b.booking_code, c.full_name AS customer_name, i.title AS item_title "
       << "FROM invoices inv "
       << "JOIN bookings b ON inv.booking_id = b.id "
       << "JOIN customers c ON b.customer_id = c.id "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "WHERE inv.booking_id = " << bookingId << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto invoice = std::make_shared<Invoice>(
        r.getInt("id"), r.getString("invoice_number"), r.getInt("booking_id"),
        r.getString("issue_date"), r.getString("due_date"), r.getDouble("subtotal"),
        r.getDouble("discount_amount"), r.getDouble("tax_amount"),
        r.getDouble("security_deposit"), r.getDouble("total_amount"),
        r.getDouble("paid_amount"), r.getString("payment_status"),
        r.getString("created_at")
    );
    invoice->setBookingCode(r.getString("booking_code"));
    invoice->setCustomerName(r.getString("customer_name"));
    invoice->setItemTitle(r.getString("item_title"));
    return invoice;
}

std::shared_ptr<RentalReturn> PaymentRepository::processReturn(
    int bookingId,
    const std::string& actualReturnDate,
    double damageCharges,
    const std::string& condition,
    const std::string& notes,
    int adminId
) {
    std::ostringstream ssBk;
    ssBk << "SELECT b.*, i.daily_rate, i.security_deposit AS item_deposit, i.id AS item_id "
         << "FROM bookings b "
         << "JOIN rental_items i ON b.item_id = i.id "
         << "WHERE b.id = " << bookingId << " LIMIT 1;";
    QueryResult bkRes = m_db.executeQuery(ssBk.str());
    if (bkRes.empty()) throw NotFoundException("Booking not found: " + std::to_string(bookingId));

    const auto& b = bkRes[0];
    std::string expectedDate = b.getString("end_date");
    int itemId = b.getInt("item_id");
    double depositHeld = b.getDouble("security_deposit");
    double dailyRate = b.getDouble("daily_rate");

    // Overdue Calculation via MySQL DATEDIFF
    std::ostringstream ssDiff;
    ssDiff << "SELECT DATEDIFF('" << m_db.escapeString(actualReturnDate) << "', '"
           << m_db.escapeString(expectedDate) << "') AS diff_days;";
    QueryResult diffRes = m_db.executeQuery(ssDiff.str());
    int diffDays = diffRes.empty() ? 0 : diffRes[0].getInt("diff_days");
    int overdueDays = std::max(0, diffDays);

    double dailyLateFee = dailyRate * 1.25; // 25% surcharge on late daily rate
    double totalLateFee = 0.0;
    double netRefund = 0.0;

    RentalReturn::computeSettlement(depositHeld, overdueDays, dailyLateFee, damageCharges, totalLateFee, netRefund);
    std::string returnCode = RentalReturn::generateReturnCode();

    TransactionGuard guard(m_db);

    std::ostringstream ssIns;
    ssIns << "INSERT INTO returns (booking_id, return_code, actual_return_date, expected_return_date, "
          << "overdue_days, daily_late_fee, total_late_fee, damage_charges, item_condition, notes, "
          << "deposit_refunded, processed_by_admin_id) VALUES ("
          << bookingId << ", '" << returnCode << "', '" << m_db.escapeString(actualReturnDate) << "', '"
          << m_db.escapeString(expectedDate) << "', " << overdueDays << ", " << dailyLateFee << ", "
          << totalLateFee << ", " << damageCharges << ", '" << m_db.escapeString(condition) << "', '"
          << m_db.escapeString(notes) << "', " << netRefund << ", " << adminId << ");";
    m_db.executeUpdate(ssIns.str());
    int returnId = static_cast<int>(m_db.getLastInsertId());

    // Mark booking as completed
    std::ostringstream ssBkUpd;
    ssBkUpd << "UPDATE bookings SET booking_status = 'completed' WHERE id = " << bookingId << ";";
    m_db.executeUpdate(ssBkUpd.str());

    // Mark rental item as available
    std::ostringstream ssItUpd;
    ssItUpd << "UPDATE rental_items SET status = 'available' WHERE id = " << itemId << ";";
    m_db.executeUpdate(ssItUpd.str());

    // Record security deposit refund if any
    if (netRefund > 0.0) {
        std::string refTxn = "TXN-REF-" + returnCode.substr(4);
        std::ostringstream ssRef;
        ssRef << "INSERT INTO payments (transaction_id, booking_id, customer_id, amount, payment_method, payment_type, payment_status, remarks) "
              << "VALUES ('" << refTxn << "', " << bookingId << ", " << b.getInt("customer_id") << ", " << netRefund
              << ", 'UPI Demo', 'Deposit Refund', 'refunded', 'Deposit refund after check-in settlement');";
        m_db.executeUpdate(ssRef.str());
    }

    guard.commit();

    return std::make_shared<RentalReturn>(
        returnId, bookingId, returnCode, actualReturnDate, expectedDate,
        overdueDays, dailyLateFee, totalLateFee, damageCharges,
        condition, notes, netRefund, adminId
    );
}

std::shared_ptr<RentalReturn> PaymentRepository::findReturnByBookingId(int bookingId) {
    std::ostringstream ss;
    ss << "SELECT r.*, b.booking_code, i.title AS item_title, c.full_name AS customer_name "
       << "FROM returns r "
       << "JOIN bookings b ON r.booking_id = b.id "
       << "JOIN rental_items i ON b.item_id = i.id "
       << "JOIN customers c ON b.customer_id = c.id "
       << "WHERE r.booking_id = " << bookingId << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const auto& r = res[0];
    auto ret = std::make_shared<RentalReturn>(
        r.getInt("id"), r.getInt("booking_id"), r.getString("return_code"),
        r.getString("actual_return_date"), r.getString("expected_return_date"),
        r.getInt("overdue_days"), r.getDouble("daily_late_fee"), r.getDouble("total_late_fee"),
        r.getDouble("damage_charges"), r.getString("item_condition"),
        r.getString("notes"), r.getDouble("deposit_refunded"),
        r.getInt("processed_by_admin_id"), r.getString("created_at")
    );
    ret->setBookingCode(r.getString("booking_code"));
    ret->setItemTitle(r.getString("item_title"));
    ret->setCustomerName(r.getString("customer_name"));
    return ret;
}

std::vector<std::shared_ptr<RentalReturn>> PaymentRepository::findAllReturns() {
    std::vector<std::shared_ptr<RentalReturn>> list;
    std::string sql = "SELECT r.*, b.booking_code, i.title AS item_title, c.full_name AS customer_name "
                      "FROM returns r "
                      "JOIN bookings b ON r.booking_id = b.id "
                      "JOIN rental_items i ON b.item_id = i.id "
                      "JOIN customers c ON b.customer_id = c.id "
                      "ORDER BY r.id DESC;";

    QueryResult res = m_db.executeQuery(sql);
    for (const auto& r : res.getRows()) {
        auto ret = std::make_shared<RentalReturn>(
            r.getInt("id"), r.getInt("booking_id"), r.getString("return_code"),
            r.getString("actual_return_date"), r.getString("expected_return_date"),
            r.getInt("overdue_days"), r.getDouble("daily_late_fee"), r.getDouble("total_late_fee"),
            r.getDouble("damage_charges"), r.getString("item_condition"),
            r.getString("notes"), r.getDouble("deposit_refunded"),
            r.getInt("processed_by_admin_id"), r.getString("created_at")
        );
        ret->setBookingCode(r.getString("booking_code"));
        ret->setItemTitle(r.getString("item_title"));
        ret->setCustomerName(r.getString("customer_name"));
        list.push_back(ret);
    }
    return list;
}

FinancialSummary PaymentRepository::getFinancialSummary() {
    FinancialSummary summary;

    // 1. Earned Rent Revenue = sum of base_rent - discount_amount for completed or active bookings
    QueryResult rentRes = m_db.executeQuery(
        "SELECT COALESCE(SUM(base_rent - discount_amount), 0.0) AS earned_rent "
        "FROM bookings WHERE booking_status IN ('confirmed', 'active', 'completed');"
    );
    if (!rentRes.empty()) summary.totalEarnedRentalRevenue = rentRes[0].getDouble("earned_rent");

    // 2. Active deposits held in custody
    QueryResult depRes = m_db.executeQuery(
        "SELECT COALESCE(SUM(security_deposit), 0.0) AS deposits_held "
        "FROM bookings WHERE booking_status IN ('confirmed', 'active');"
    );
    if (!depRes.empty()) summary.totalDepositsHeld = depRes[0].getDouble("deposits_held");

    // 3. Deposits refunded
    QueryResult refRes = m_db.executeQuery(
        "SELECT COALESCE(SUM(deposit_refunded), 0.0) AS deposits_ref "
        "FROM returns;"
    );
    if (!refRes.empty()) summary.totalDepositsRefunded = refRes[0].getDouble("deposits_ref");

    // 4. Late fees & Damage charges
    QueryResult feeRes = m_db.executeQuery(
        "SELECT COALESCE(SUM(total_late_fee), 0.0) AS late_fee, "
        "COALESCE(SUM(damage_charges), 0.0) AS damage "
        "FROM returns;"
    );
    if (!feeRes.empty()) {
        summary.totalLateFeesCollected = feeRes[0].getDouble("late_fee");
        summary.totalDamageChargesCollected = feeRes[0].getDouble("damage");
    }

    // Net Earned Revenue excludes security deposits (which are liabilities, not earned revenue)
    summary.netTotalRevenue = summary.totalEarnedRentalRevenue + summary.totalLateFeesCollected + summary.totalDamageChargesCollected;

    // Transaction count
    QueryResult txnRes = m_db.executeQuery("SELECT COUNT(*) AS total_txns FROM payments;");
    if (!txnRes.empty()) summary.totalTransactionsCount = txnRes[0].getInt("total_txns");

    return summary;
}

std::vector<std::map<std::string, std::string>> PaymentRepository::getMonthlyRevenueStats() {
    std::vector<std::map<std::string, std::string>> stats;
    std::string sql = "SELECT DATE_FORMAT(created_at, '%b %Y') AS month_label, "
                      "COALESCE(SUM(base_rent - discount_amount), 0.0) AS monthly_revenue, "
                      "COUNT(*) AS booking_count "
                      "FROM bookings "
                      "WHERE booking_status IN ('confirmed', 'active', 'completed') "
                      "GROUP BY DATE_FORMAT(created_at, '%Y-%m'), DATE_FORMAT(created_at, '%b %Y') "
                      "ORDER BY DATE_FORMAT(created_at, '%Y-%m') ASC "
                      "LIMIT 6;";

    QueryResult res = m_db.executeQuery(sql);
    for (const auto& r : res.getRows()) {
        std::map<std::string, std::string> m;
        m["month"] = r.getString("month_label");
        m["revenue"] = r.getString("monthly_revenue");
        m["bookings"] = r.getString("booking_count");
        stats.push_back(m);
    }
    return stats;
}
