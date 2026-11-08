#ifndef PAYMENT_REPOSITORY_H
#define PAYMENT_REPOSITORY_H

#include "../models/Payment.h"
#include "../models/Invoice.h"
#include "../models/RentalReturn.h"
#include "Database.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

struct FinancialSummary {
    double totalEarnedRentalRevenue = 0.0;
    double totalDepositsHeld = 0.0;
    double totalDepositsRefunded = 0.0;
    double totalLateFeesCollected = 0.0;
    double totalDamageChargesCollected = 0.0;
    double netTotalRevenue = 0.0;
    int totalTransactionsCount = 0;
};

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation, Financial Integrity & Return Management
 * PaymentRepository processes demo transactions, manages billing invoices,
 * records check-in item returns, and calculates accurate revenue metrics.
 * ============================================================================
 */
class PaymentRepository {
private:
    Database& m_db;

public:
    explicit PaymentRepository(Database& db = Database::getInstance());

    std::shared_ptr<Payment> recordPayment(
        int bookingId,
        int customerId,
        double amount,
        const std::string& method,
        const std::string& type = "Rental Payment",
        const std::string& remarks = "Demo payment successful"
    );

    std::shared_ptr<Payment> findById(int id);
    std::shared_ptr<Payment> findByTransactionId(const std::string& txnId);
    std::vector<std::shared_ptr<Payment>> findByBookingId(int bookingId);
    std::vector<std::shared_ptr<Payment>> findByCustomerId(int customerId);
    std::vector<std::shared_ptr<Payment>> findAllPayments();

    // Invoice Management
    std::shared_ptr<Invoice> createOrGetInvoice(int bookingId);
    std::shared_ptr<Invoice> findInvoiceByBookingId(int bookingId);

    // Return & Check-in Management
    std::shared_ptr<RentalReturn> processReturn(
        int bookingId,
        const std::string& actualReturnDate,
        double damageCharges = 0.0,
        const std::string& condition = "Good",
        const std::string& notes = "",
        int adminId = 1
    );

    std::shared_ptr<RentalReturn> findReturnByBookingId(int bookingId);
    std::vector<std::shared_ptr<RentalReturn>> findAllReturns();

    // Financial Analytics
    FinancialSummary getFinancialSummary();
    std::vector<std::map<std::string, std::string>> getMonthlyRevenueStats();
};

#endif // PAYMENT_REPOSITORY_H
