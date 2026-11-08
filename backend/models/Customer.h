#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Single / Multilevel Inheritance, Method Overriding
 * Customer derives from User, inheriting authentication attributes
 * and adding customer profile data (full name, phone, address, licenses).
 * ============================================================================
 */
class Customer : public User {
private:
    int m_customerId;
    std::string m_fullName;
    std::string m_phone;
    std::string m_address;
    std::string m_idProofNumber;
    std::string m_drivingLicenseNumber;

public:
    // Default Constructor
    Customer();

    // Parameterized Constructor (Invokes Base Class Constructor)
    Customer(int userId, const std::string& email, const std::string& passwordHash,
             int customerId, const std::string& fullName, const std::string& phone,
             const std::string& address, const std::string& idProof = "",
             const std::string& drivingLicense = "", bool isActive = true,
             const std::string& createdAt = "");

    // Copy Constructor
    Customer(const Customer& other);

    // Virtual Destructor
    virtual ~Customer() override;

    // Getters
    int getCustomerId() const;
    std::string getFullName() const;
    std::string getPhone() const;
    std::string getAddress() const;
    std::string getIdProofNumber() const;
    std::string getDrivingLicenseNumber() const;

    // Setters with validation
    void setCustomerId(int id);
    void setFullName(const std::string& name);
    void setPhone(const std::string& phone);
    void setAddress(const std::string& address);
    void setIdProofNumber(const std::string& proof);
    void setDrivingLicenseNumber(const std::string& dl);

    // Overriding polymorphic methods
    virtual std::string getDisplaySummary() const override;
    virtual std::string toJson() const override;
};

#endif // CUSTOMER_H
