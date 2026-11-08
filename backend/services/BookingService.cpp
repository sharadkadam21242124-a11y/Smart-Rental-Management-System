#include "BookingService.h"
#include "../utils/Exceptions.h"

BookingService::BookingService(BookingRepository& bRepo, RentalRepository& rRepo)
    : m_bookingRepo(bRepo), m_rentalRepo(rRepo) {}

bool BookingService::checkAvailability(int itemId, const std::string& startDate, const std::string& endDate) {
    if (startDate >= endDate) return false;
    return m_bookingRepo.isItemAvailableForDates(itemId, startDate, endDate);
}

std::shared_ptr<Booking> BookingService::createBooking(int customerId, int itemId,
                                                      const std::string& startDate,
                                                      const std::string& endDate) {
    return m_bookingRepo.createBooking(customerId, itemId, startDate, endDate);
}

std::shared_ptr<Booking> BookingService::getBookingById(int id) {
    auto b = m_bookingRepo.findById(id);
    if (!b) throw NotFoundException("Booking not found: " + std::to_string(id));
    return b;
}

std::shared_ptr<Booking> BookingService::getBookingByCode(const std::string& code) {
    auto b = m_bookingRepo.findByCode(code);
    if (!b) throw NotFoundException("Booking not found: " + code);
    return b;
}

std::vector<std::shared_ptr<Booking>> BookingService::getCustomerBookings(int customerId) {
    return m_bookingRepo.findByCustomerId(customerId);
}

std::vector<std::shared_ptr<Booking>> BookingService::getAllBookings(const std::string& status) {
    return m_bookingRepo.findAllBookings(status);
}

bool BookingService::cancelBooking(int bookingId, int customerId) {
    return m_bookingRepo.cancelBooking(bookingId, customerId);
}

bool BookingService::confirmBooking(int bookingId) {
    return m_bookingRepo.confirmBooking(bookingId);
}
