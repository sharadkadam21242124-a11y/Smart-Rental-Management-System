#ifndef BOOKING_H
#define BOOKING_H

#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Composition, Operator Overloading, Encapsulation
 * Booking aggregates customer and rental item relationships, maintaining
 * calculated financial state and lease scheduling.
 * ============================================================================
 */
class Booking {
private:
    int m_id;
    std::string m_bookingCode;
    int m_customerId;
    int m_itemId;
    std::string m_startDate;
    std::string m_endDate;
    int m_rentalDays;
    double m_dailyRate;
    double m_baseRent;
    double m_discountRate;
    double m_discountAmount;
    double m_securityDeposit;
    double m_totalAmount;
    std::string m_bookingStatus; // pending, confirmed, active, completed, cancelled
    std::string m_createdAt;
    std::string m_updatedAt;

    // Optional cached titles for UI convenience
    std::string m_itemTitle;
    std::string m_customerName;
    std::string m_category;

public:
    Booking();
    Booking(int id, const std::string& code, int customerId, int itemId,
            const std::string& startDate, const std::string& endDate,
            int rentalDays, double dailyRate, double baseRent,
            double discountRate, double discountAmount, double securityDeposit,
            double totalAmount, const std::string& status = "pending",
            const std::string& createdAt = "", const std::string& updatedAt = "");

    // Getters
    int getId() const;
    std::string getBookingCode() const;
    int getCustomerId() const;
    int getItemId() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    int getRentalDays() const;
    double getDailyRate() const;
    double getBaseRent() const;
    double getDiscountRate() const;
    double getDiscountAmount() const;
    double getSecurityDeposit() const;
    double getTotalAmount() const;
    std::string getBookingStatus() const;
    std::string getCreatedAt() const;
    std::string getUpdatedAt() const;

    std::string getItemTitle() const;
    std::string getCustomerName() const;
    std::string getCategory() const;

    // Setters
    void setId(int id);
    void setBookingCode(const std::string& code);
    void setCustomerId(int id);
    void setItemId(int id);
    void setStartDate(const std::string& start);
    void setEndDate(const std::string& end);
    void setRentalDays(int days);
    void setDailyRate(double rate);
    void setBaseRent(double rent);
    void setDiscountRate(double rate);
    void setDiscountAmount(double amount);
    void setSecurityDeposit(double deposit);
    void setTotalAmount(double total);
    void setBookingStatus(const std::string& status);
    void setCreatedAt(const std::string& created);
    void setUpdatedAt(const std::string& updated);

    void setItemTitle(const std::string& title);
    void setCustomerName(const std::string& name);
    void setCategory(const std::string& cat);

    // Operator Overloading: Compare bookings by total financial magnitude
    bool operator<(const Booking& other) const;
    bool operator==(const Booking& other) const;

    std::string toJson() const;
};

#endif // BOOKING_H
