#ifndef BOOKING_REPOSITORY_H
#define BOOKING_REPOSITORY_H

#include "../models/Booking.h"
#include "Database.h"
#include <memory>
#include <vector>
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Business Rule Enforcement & Transaction Management
 * BookingRepository prevents double-booking overlaps, enforces rental rules,
 * computes duration discounts, and manages booking lifecycle transitions.
 * ============================================================================
 */
class BookingRepository {
private:
    Database& m_db;

public:
    explicit BookingRepository(Database& db = Database::getInstance());

    // Conflict detection
    bool isItemAvailableForDates(int itemId, const std::string& startDate, const std::string& endDate, int excludeBookingId = 0);

    // Create booking transaction
    std::shared_ptr<Booking> createBooking(int customerId, int itemId,
                                          const std::string& startDate,
                                          const std::string& endDate);

    std::shared_ptr<Booking> findById(int id);
    std::shared_ptr<Booking> findByCode(const std::string& code);
    std::vector<std::shared_ptr<Booking>> findByCustomerId(int customerId);
    std::vector<std::shared_ptr<Booking>> findAllBookings(const std::string& status = "");
    std::vector<std::shared_ptr<Booking>> findActiveRentals();

    bool cancelBooking(int bookingId, int customerId = 0);
    bool confirmBooking(int bookingId);
    bool updateStatus(int bookingId, const std::string& status);

    // Metrics
    int getTotalBookingCount();
    int getActiveBookingCount();
    int getPendingBookingCount();
};

#endif // BOOKING_REPOSITORY_H
