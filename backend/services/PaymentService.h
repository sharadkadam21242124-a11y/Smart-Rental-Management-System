#ifndef PAYMENT_SERVICE_H
#define PAYMENT_SERVICE_H

#include "../database/PaymentRepository.h"
#include <memory>
#include <vector>
#include <string>

class PaymentService {
private:
    PaymentRepository& m_payRepo;

public:
    explicit PaymentService(PaymentRepository& repo);

    std::shared_ptr<Payment> processDemoPayment(
        int bookingId,
        int customerId,
        double amount,
        const std::string& method,
        const std::string& type = "Rental Payment",
        const std::string& remarks = "Demo Transaction Success"
    );

    std::shared_ptr<Payment> getPaymentById(int id);
    std::shared_ptr<Payment> getPaymentByTransactionId(const std::string& txnId);
    std::vector<std::shared_ptr<Payment>> getCustomerPayments(int customerId);
    std::vector<std::shared_ptr<Payment>> getAllPayments();
};

#endif // PAYMENT_SERVICE_H
