#include "Payment.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>
#include <chrono>
#include <random>

Payment::Payment()
    : m_id(0), m_transactionId(""), m_bookingId(0), m_customerId(0),
      m_amount(0.0), m_paymentMethod("UPI Demo"), m_paymentType("Rental Payment"),
      m_paymentStatus("success"), m_paymentDate(""),
      m_remarks("Demo Transaction Processed"), m_customerName(""), m_bookingCode("") {}

Payment::Payment(int id, const std::string& transId, int bookingId, int customerId,
                 double amount, const std::string& method,
                 const std::string& type, const std::string& status,
                 const std::string& date, const std::string& remarks)
    : m_id(id), m_transactionId(transId), m_bookingId(bookingId),
      m_customerId(customerId), m_amount(amount), m_paymentMethod(method),
      m_paymentType(type), m_paymentStatus(status), m_paymentDate(date),
      m_remarks(remarks), m_customerName(""), m_bookingCode("") {
    if (amount < 0.0) throw ValidationException("Payment amount cannot be negative.");
}

std::string Payment::generateTransactionId(const std::string& method) {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1000, 9999);

    std::string prefix = "TXN-DEMO-";
    if (method == "UPI Demo") prefix = "TXN-UPI-";
    else if (method == "Card Demo") prefix = "TXN-CRD-";
    else if (method == "Cash") prefix = "TXN-CSH-";

    std::ostringstream ss;
    ss << prefix << (now % 10000000) << dis(gen);
    return ss.str();
}

int Payment::getId() const { return m_id; }
std::string Payment::getTransactionId() const { return m_transactionId; }
int Payment::getBookingId() const { return m_bookingId; }
int Payment::getCustomerId() const { return m_customerId; }
double Payment::getAmount() const { return m_amount; }
std::string Payment::getPaymentMethod() const { return m_paymentMethod; }
std::string Payment::getPaymentType() const { return m_paymentType; }
std::string Payment::getPaymentStatus() const { return m_paymentStatus; }
std::string Payment::getPaymentDate() const { return m_paymentDate; }
std::string Payment::getRemarks() const { return m_remarks; }
std::string Payment::getCustomerName() const { return m_customerName; }
std::string Payment::getBookingCode() const { return m_bookingCode; }

void Payment::setId(int id) { m_id = id; }
void Payment::setTransactionId(const std::string& id) { m_transactionId = id; }
void Payment::setBookingId(int id) { m_bookingId = id; }
void Payment::setCustomerId(int id) { m_customerId = id; }
void Payment::setAmount(double amt) { m_amount = amt; }
void Payment::setPaymentMethod(const std::string& method) { m_paymentMethod = method; }
void Payment::setPaymentType(const std::string& type) { m_paymentType = type; }
void Payment::setPaymentStatus(const std::string& status) { m_paymentStatus = status; }
void Payment::setPaymentDate(const std::string& date) { m_paymentDate = date; }
void Payment::setRemarks(const std::string& remarks) { m_remarks = remarks; }
void Payment::setCustomerName(const std::string& name) { m_customerName = name; }
void Payment::setBookingCode(const std::string& code) { m_bookingCode = code; }

std::string Payment::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("transaction_id", m_transactionId)
       << JsonHelper::kv("booking_id", m_bookingId)
       << JsonHelper::kv("customer_id", m_customerId)
       << JsonHelper::kvString("booking_code", m_bookingCode)
       << JsonHelper::kvString("customer_name", m_customerName)
       << JsonHelper::kv("amount", m_amount)
       << JsonHelper::kvString("payment_method", m_paymentMethod)
       << JsonHelper::kvString("payment_type", m_paymentType)
       << JsonHelper::kvString("payment_status", m_paymentStatus)
       << JsonHelper::kvString("payment_date", m_paymentDate)
       << JsonHelper::kvString("remarks", m_remarks, false)
       << "}";
    return ss.str();
}
