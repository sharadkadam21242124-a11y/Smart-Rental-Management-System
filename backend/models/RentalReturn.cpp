#include "RentalReturn.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>
#include <chrono>
#include <random>
#include <algorithm>

RentalReturn::RentalReturn()
    : m_id(0), m_bookingId(0), m_returnCode(""), m_actualReturnDate(""),
      m_expectedReturnDate(""), m_overdueDays(0), m_dailyLateFee(0.0),
      m_totalLateFee(0.0), m_damageCharges(0.0), m_itemCondition("Good"),
      m_notes(""), m_depositRefunded(0.0), m_processedByAdminId(1),
      m_createdAt(""), m_bookingCode(""), m_itemTitle(""), m_customerName("") {}

RentalReturn::RentalReturn(int id, int bookingId, const std::string& code,
                           const std::string& actualDate, const std::string& expectedDate,
                           int overdueDays, double dailyLateFee, double totalLateFee,
                           double damageCharges, const std::string& condition,
                           const std::string& notes, double depositRefunded,
                           int processedByAdminId, const std::string& createdAt)
    : m_id(id), m_bookingId(bookingId), m_returnCode(code),
      m_actualReturnDate(actualDate), m_expectedReturnDate(expectedDate),
      m_overdueDays(overdueDays), m_dailyLateFee(dailyLateFee),
      m_totalLateFee(totalLateFee), m_damageCharges(damageCharges),
      m_itemCondition(condition), m_notes(notes),
      m_depositRefunded(depositRefunded),
      m_processedByAdminId(processedByAdminId), m_createdAt(createdAt),
      m_bookingCode(""), m_itemTitle(""), m_customerName("") {}

std::string RentalReturn::generateReturnCode() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, 999);
    std::ostringstream ss;
    ss << "RET-2026-" << (now % 100000) << dis(gen);
    return ss.str();
}

void RentalReturn::computeSettlement(double deposit, int overdueDays, double dailyLateRate,
                                     double damageCharges, double& outLateFee, double& outRefund) {
    if (overdueDays > 0) {
        outLateFee = overdueDays * dailyLateRate;
    } else {
        outLateFee = 0.0;
    }

    double deductions = outLateFee + damageCharges;
    outRefund = std::max(0.0, deposit - deductions);
}

int RentalReturn::getId() const { return m_id; }
int RentalReturn::getBookingId() const { return m_bookingId; }
std::string RentalReturn::getReturnCode() const { return m_returnCode; }
std::string RentalReturn::getActualReturnDate() const { return m_actualReturnDate; }
std::string RentalReturn::getExpectedReturnDate() const { return m_expectedReturnDate; }
int RentalReturn::getOverdueDays() const { return m_overdueDays; }
double RentalReturn::getDailyLateFee() const { return m_dailyLateFee; }
double RentalReturn::getTotalLateFee() const { return m_totalLateFee; }
double RentalReturn::getDamageCharges() const { return m_damageCharges; }
std::string RentalReturn::getItemCondition() const { return m_itemCondition; }
std::string RentalReturn::getNotes() const { return m_notes; }
double RentalReturn::getDepositRefunded() const { return m_depositRefunded; }
int RentalReturn::getProcessedByAdminId() const { return m_processedByAdminId; }
std::string RentalReturn::getCreatedAt() const { return m_createdAt; }

std::string RentalReturn::getBookingCode() const { return m_bookingCode; }
std::string RentalReturn::getItemTitle() const { return m_itemTitle; }
std::string RentalReturn::getCustomerName() const { return m_customerName; }

void RentalReturn::setId(int id) { m_id = id; }
void RentalReturn::setBookingId(int id) { m_bookingId = id; }
void RentalReturn::setReturnCode(const std::string& code) { m_returnCode = code; }
void RentalReturn::setActualReturnDate(const std::string& d) { m_actualReturnDate = d; }
void RentalReturn::setExpectedReturnDate(const std::string& d) { m_expectedReturnDate = d; }
void RentalReturn::setOverdueDays(int days) { m_overdueDays = days; }
void RentalReturn::setDailyLateFee(double fee) { m_dailyLateFee = fee; }
void RentalReturn::setTotalLateFee(double fee) { m_totalLateFee = fee; }
void RentalReturn::setDamageCharges(double charges) { m_damageCharges = charges; }
void RentalReturn::setItemCondition(const std::string& condition) { m_itemCondition = condition; }
void RentalReturn::setNotes(const std::string& notes) { m_notes = notes; }
void RentalReturn::setDepositRefunded(double refund) { m_depositRefunded = refund; }
void RentalReturn::setProcessedByAdminId(int adminId) { m_processedByAdminId = adminId; }
void RentalReturn::setCreatedAt(const std::string& created) { m_createdAt = created; }

void RentalReturn::setBookingCode(const std::string& code) { m_bookingCode = code; }
void RentalReturn::setItemTitle(const std::string& title) { m_itemTitle = title; }
void RentalReturn::setCustomerName(const std::string& name) { m_customerName = name; }

std::string RentalReturn::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kv("booking_id", m_bookingId)
       << JsonHelper::kvString("return_code", m_returnCode)
       << JsonHelper::kvString("booking_code", m_bookingCode)
       << JsonHelper::kvString("item_title", m_itemTitle)
       << JsonHelper::kvString("customer_name", m_customerName)
       << JsonHelper::kvString("actual_return_date", m_actualReturnDate)
       << JsonHelper::kvString("expected_return_date", m_expectedReturnDate)
       << JsonHelper::kv("overdue_days", m_overdueDays)
       << JsonHelper::kv("daily_late_fee", m_dailyLateFee)
       << JsonHelper::kv("total_late_fee", m_totalLateFee)
       << JsonHelper::kv("damage_charges", m_damageCharges)
       << JsonHelper::kvString("item_condition", m_itemCondition)
       << JsonHelper::kvString("notes", m_notes)
       << JsonHelper::kv("deposit_refunded", m_depositRefunded)
       << JsonHelper::kv("processed_by_admin_id", m_processedByAdminId)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
