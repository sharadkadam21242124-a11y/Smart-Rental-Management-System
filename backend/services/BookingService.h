#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include "../database/BookingRepository.h"
#include "../database/RentalRepository.h"
#include <memory>
#include <vector>
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Service Layer Pattern & Domain Logic Coordination
 * BookingService encapsulates date conflict checking, booking creation,
 * cancellation policy verification, and active rental tracking.
 * ============================================================================
 */
class BookingService {
private:
    BookingRepository& m_bookingRepo;
    RentalRepository& m_rentalRepo;

public:
    BookingService(BookingRepository& bRepo, RentalRepository& rRepo);

    bool checkAvailability(int itemId, const std::string& startDate, const std::string& endDate);

    std::shared_ptr<Booking> createBooking(int customerId, int itemId,
                                          const std::string& startDate,
                                          const std::string& endDate);

    std::shared_ptr<Booking> getBookingById(int id);
    std::shared_ptr<Booking> getBookingByCode(const std::string& code);
    std::vector<std::shared_ptr<Booking>> getCustomerBookings(int customerId);
    std::vector<std::shared_ptr<Booking>> getAllBookings(const std::string& status = "");

    bool cancelBooking(int bookingId, int customerId = 0);
    bool confirmBooking(int bookingId);
};

#endif // BOOKING_SERVICE_H
