#include "Invoice.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>
#include <chrono>
#include <random>

Invoice::Invoice()
    : m_id(0), m_invoiceNumber(""), m_bookingId(0), m_issueDate(""),
      m_dueDate(""), m_subtotal(0.0), m_discountAmount(0.0), m_taxAmount(0.0),
      m_securityDeposit(0.0), m_totalAmount(0.0), m_paidAmount(0.0),
      m_paymentStatus("unpaid"), m_createdAt(""), m_bookingCode(""),
      m_customerName(""), m_itemTitle("") {}

Invoice::Invoice(int id, const std::string& invNum, int bookingId,
                 const std::string& issueDate, const std::string& dueDate,
                 double subtotal, double discount, double tax,
                 double deposit, double total, double paid,
                 const std::string& status, const std::string& createdAt)
    : m_id(id), m_invoiceNumber(invNum), m_bookingId(bookingId),
      m_issueDate(issueDate), m_dueDate(dueDate), m_subtotal(subtotal),
      m_discountAmount(discount), m_taxAmount(tax), m_securityDeposit(deposit),
      m_totalAmount(total), m_paidAmount(paid), m_paymentStatus(status),
      m_createdAt(createdAt), m_bookingCode(""), m_customerName(""), m_itemTitle("") {}

std::string Invoice::generateInvoiceNumber() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, 999);
    std::ostringstream ss;
    ss << "INV-2026-" << (now % 100000) << dis(gen);
    return ss.str();
}

int Invoice::getId() const { return m_id; }
std::string Invoice::getInvoiceNumber() const { return m_invoiceNumber; }
int Invoice::getBookingId() const { return m_bookingId; }
std::string Invoice::getIssueDate() const { return m_issueDate; }
std::string Invoice::getDueDate() const { return m_dueDate; }
double Invoice::getSubtotal() const { return m_subtotal; }
double Invoice::getDiscountAmount() const { return m_discountAmount; }
double Invoice::getTaxAmount() const { return m_taxAmount; }
double Invoice::getSecurityDeposit() const { return m_securityDeposit; }
double Invoice::getTotalAmount() const { return m_totalAmount; }
double Invoice::getPaidAmount() const { return m_paidAmount; }
std::string Invoice::getPaymentStatus() const { return m_paymentStatus; }
std::string Invoice::getCreatedAt() const { return m_createdAt; }

std::string Invoice::getBookingCode() const { return m_bookingCode; }
std::string Invoice::getCustomerName() const { return m_customerName; }
std::string Invoice::getItemTitle() const { return m_itemTitle; }

void Invoice::setId(int id) { m_id = id; }
void Invoice::setInvoiceNumber(const std::string& num) { m_invoiceNumber = num; }
void Invoice::setBookingId(int id) { m_bookingId = id; }
void Invoice::setIssueDate(const std::string& d) { m_issueDate = d; }
void Invoice::setDueDate(const std::string& d) { m_dueDate = d; }
void Invoice::setSubtotal(double amt) { m_subtotal = amt; }
void Invoice::setDiscountAmount(double amt) { m_discountAmount = amt; }
void Invoice::setTaxAmount(double amt) { m_taxAmount = amt; }
void Invoice::setSecurityDeposit(double amt) { m_securityDeposit = amt; }
void Invoice::setTotalAmount(double amt) { m_totalAmount = amt; }
void Invoice::setPaidAmount(double amt) { m_paidAmount = amt; }
void Invoice::setPaymentStatus(const std::string& status) { m_paymentStatus = status; }
void Invoice::setCreatedAt(const std::string& created) { m_createdAt = created; }

void Invoice::setBookingCode(const std::string& code) { m_bookingCode = code; }
void Invoice::setCustomerName(const std::string& name) { m_customerName = name; }
void Invoice::setItemTitle(const std::string& title) { m_itemTitle = title; }

std::string Invoice::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("invoice_number", m_invoiceNumber)
       << JsonHelper::kv("booking_id", m_bookingId)
       << JsonHelper::kvString("booking_code", m_bookingCode)
       << JsonHelper::kvString("customer_name", m_customerName)
       << JsonHelper::kvString("item_title", m_itemTitle)
       << JsonHelper::kvString("issue_date", m_issueDate)
       << JsonHelper::kvString("due_date", m_dueDate)
       << JsonHelper::kv("subtotal", m_subtotal)
       << JsonHelper::kv("discount_amount", m_discountAmount)
       << JsonHelper::kv("tax_amount", m_taxAmount)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kv("total_amount", m_totalAmount)
       << JsonHelper::kv("paid_amount", m_paidAmount)
       << JsonHelper::kvString("payment_status", m_paymentStatus)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
