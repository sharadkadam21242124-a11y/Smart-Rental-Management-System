#include "ReturnService.h"
#include "../utils/Exceptions.h"

ReturnService::ReturnService(PaymentRepository& repo) : m_payRepo(repo) {}

std::shared_ptr<RentalReturn> ReturnService::processCheckIn(
    int bookingId,
    const std::string& actualReturnDate,
    double damageCharges,
    const std::string& condition,
    const std::string& notes,
    int adminId
) {
    if (actualReturnDate.empty()) {
        throw ValidationException("Actual return date cannot be empty.");
    }
    return m_payRepo.processReturn(bookingId, actualReturnDate, damageCharges, condition, notes, adminId);
}

std::shared_ptr<RentalReturn> ReturnService::getReturnByBookingId(int bookingId) {
    auto r = m_payRepo.findReturnByBookingId(bookingId);
    if (!r) throw NotFoundException("Return record not found for booking: " + std::to_string(bookingId));
    return r;
}

std::vector<std::shared_ptr<RentalReturn>> ReturnService::getAllReturns() {
    return m_payRepo.findAllReturns();
}
