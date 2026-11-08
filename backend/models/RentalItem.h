#ifndef RENTAL_ITEM_H
#define RENTAL_ITEM_H

#include <string>
#include <vector>
#include <iostream>

/**
 * ============================================================================
 * OOP CONCEPT: Abstraction, Polymorphism, Operator Overloading,
 *              Friend Functions, Static Members, Virtual Destructor
 * 
 * RentalItem is an ABSTRACT BASE CLASS representing any item that can be rented.
 * It defines the polymorphic interface for Car, Bike, House, and Furniture.
 * ============================================================================
 */
class RentalItem {
protected:
    int m_id;
    std::string m_title;
    std::string m_description;
    double m_dailyRate;
    double m_securityDeposit;
    std::string m_location;
    std::string m_status; // "available", "rented", "maintenance", "retired"
    std::string m_imageUrl;
    std::string m_createdAt;

    // Static member counter to track total instances instantiated
    static int s_activeItemCount;

public:
    // Default Constructor
    RentalItem();

    // Parameterized Constructor
    RentalItem(int id, const std::string& title, const std::string& description,
               double dailyRate, double securityDeposit, const std::string& location,
               const std::string& status = "available", const std::string& imageUrl = "",
               const std::string& createdAt = "");

    // Copy Constructor
    RentalItem(const RentalItem& other);

    // Virtual Destructor (Crucial for Abstract Base Class)
    virtual ~RentalItem();

    // Getters (Const Member Functions)
    int getId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    double getDailyRate() const;
    double getSecurityDeposit() const;
    std::string getLocation() const;
    std::string getStatus() const;
    std::string getImageUrl() const;
    std::string getCreatedAt() const;

    // Setters with validation
    void setId(int id);
    void setTitle(const std::string& title);
    void setDescription(const std::string& desc);
    void setDailyRate(double rate);
    void setSecurityDeposit(double deposit);
    void setLocation(const std::string& loc);
    void setStatus(const std::string& status);
    void setImageUrl(const std::string& url);

    // Static Member Functions
    static int getActiveItemCount();
    static const std::vector<std::string>& getSupportedCategories();

    // ------------------------------------------------------------------------
    // PURE VIRTUAL FUNCTIONS (Making RentalItem an Abstract Class)
    // ------------------------------------------------------------------------
    virtual std::string getCategory() const = 0;
    virtual double calculateRent(int days) const = 0;
    virtual std::string toJson() const = 0;

    // ------------------------------------------------------------------------
    // FUNCTION OVERLOADING: Overloaded calculateRent with discount parameter
    // ------------------------------------------------------------------------
    virtual double calculateRent(int days, double discountPercentage) const;

    // Helper to calculate total booking amount = Rent + Security Deposit
    double calculateTotalPayable(int days, double discountPercentage = 0.0) const;

    // ------------------------------------------------------------------------
    // OPERATOR OVERLOADING:
    // 1. operator< : Compares daily rates for price sorting
    // 2. operator== : Compares item identity by ID
    // ------------------------------------------------------------------------
    bool operator<(const RentalItem& other) const;
    bool operator==(const RentalItem& other) const;

    // ------------------------------------------------------------------------
    // FRIEND FUNCTIONS:
    // 1. Stream insertion operator for formatted debugging
    // 2. Value comparison function
    // ------------------------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& os, const RentalItem& item);
    friend bool compareRentalValue(const RentalItem& a, const RentalItem& b);
};

#endif // RENTAL_ITEM_H
