#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H

#include "../database/PaymentRepository.h"
#include "../database/BookingRepository.h"
#include <memory>

struct BillEstimate {
    double dailyRate = 0.0;
    int rentalDays = 1;
    double baseRent = 0.0;
    double discountPercentage = 0.0;
    double discountAmount = 0.0;
    double securityDeposit = 0.0;
    double totalPayable = 0.0;
};

class BillingService {
private:
    PaymentRepository& m_payRepo;
    BookingRepository& m_bookingRepo;

public:
    BillingService(PaymentRepository& pRepo, BookingRepository& bRepo);

    static BillEstimate estimateBill(double dailyRate, int days, double deposit);
    std::shared_ptr<Invoice> generateInvoice(int bookingId);
    std::shared_ptr<Invoice> getInvoiceByBookingId(int bookingId);
    std::shared_ptr<Booking> getBookingById(int bookingId);
};

#endif // BILLING_SERVICE_H
