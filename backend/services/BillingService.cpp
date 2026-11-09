#include "BillingService.h"
#include "../utils/Exceptions.h"

BillingService::BillingService(PaymentRepository& pRepo, BookingRepository& bRepo)
    : m_payRepo(pRepo), m_bookingRepo(bRepo) {}

BillEstimate BillingService::estimateBill(double dailyRate, int days, double deposit) {
    if (days < 1) days = 1;
    BillEstimate est;
    est.dailyRate = dailyRate;
    est.rentalDays = days;
    est.baseRent = dailyRate * days;

    if (days >= 30) est.discountPercentage = 20.0;
    else if (days >= 14) est.discountPercentage = 15.0;
    else if (days >= 7) est.discountPercentage = 10.0;
    else if (days >= 5) est.discountPercentage = 5.0;
    else est.discountPercentage = 0.0;

    est.discountAmount = est.baseRent * (est.discountPercentage / 100.0);
    est.securityDeposit = deposit;
    est.totalPayable = (est.baseRent - est.discountAmount) + deposit;
    return est;
}

std::shared_ptr<Invoice> BillingService::generateInvoice(int bookingId) {
    return m_payRepo.createOrGetInvoice(bookingId);
}

std::shared_ptr<Invoice> BillingService::getInvoiceByBookingId(int bookingId) {
    auto inv = m_payRepo.findInvoiceByBookingId(bookingId);
    if (!inv) {
        return generateInvoice(bookingId);
    }
    return inv;
}

std::shared_ptr<Booking> BillingService::getBookingById(int bookingId) {
    return m_bookingRepo.findById(bookingId);
}
