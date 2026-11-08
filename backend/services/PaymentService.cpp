#include "PaymentService.h"
#include "../utils/Exceptions.h"

PaymentService::PaymentService(PaymentRepository& repo) : m_payRepo(repo) {}

std::shared_ptr<Payment> PaymentService::processDemoPayment(
    int bookingId,
    int customerId,
    double amount,
    const std::string& method,
    const std::string& type,
    const std::string& remarks
) {
    if (method != "Cash" && method != "UPI Demo" && method != "Card Demo") {
        throw ValidationException("Invalid payment method. Supported: Cash, UPI Demo, Card Demo");
    }
    return m_payRepo.recordPayment(bookingId, customerId, amount, method, type, remarks);
}

std::shared_ptr<Payment> PaymentService::getPaymentById(int id) {
    auto p = m_payRepo.findById(id);
    if (!p) throw NotFoundException("Payment record not found: " + std::to_string(id));
    return p;
}

std::shared_ptr<Payment> PaymentService::getPaymentByTransactionId(const std::string& txnId) {
    auto p = m_payRepo.findByTransactionId(txnId);
    if (!p) throw NotFoundException("Payment transaction not found: " + txnId);
    return p;
}

std::vector<std::shared_ptr<Payment>> PaymentService::getCustomerPayments(int customerId) {
    return m_payRepo.findByCustomerId(customerId);
}

std::vector<std::shared_ptr<Payment>> PaymentService::getAllPayments() {
    return m_payRepo.findAllPayments();
}
