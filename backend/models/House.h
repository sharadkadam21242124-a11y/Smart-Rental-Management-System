#ifndef HOUSE_H
#define HOUSE_H

#include "RentalItem.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Hierarchical Inheritance & Polymorphism
 * House derives from RentalItem, specialized for apartments, villas, and studios.
 * ============================================================================
 */
class House : public RentalItem {
private:
    std::string m_propertyType; // Apartment, Villa, Independent House, Studio, Penthouse
    int m_bedrooms;
    int m_bathrooms;
    std::string m_furnishing; // Furnished, Semi-Furnished, Unfurnished
    int m_areaSqft;
    std::string m_addressLine;
    bool m_parkingAvailable;

public:
    House();
    House(int id, const std::string& title, const std::string& description,
          double dailyRate, double securityDeposit, const std::string& location,
          const std::string& propertyType, int bedrooms, int bathrooms,
          const std::string& furnishing, int areaSqft,
          const std::string& addressLine, bool parkingAvailable = true,
          const std::string& status = "available",
          const std::string& imageUrl = "",
          const std::string& createdAt = "");
    virtual ~House() override;

    // Getters
    std::string getPropertyType() const;
    int getBedrooms() const;
    int getBathrooms() const;
    std::string getFurnishing() const;
    int getAreaSqft() const;
    std::string getAddressLine() const;
    bool isParkingAvailable() const;

    // Setters
    void setPropertyType(const std::string& type);
    void setBedrooms(int beds);
    void setBathrooms(int baths);
    void setFurnishing(const std::string& furnishing);
    void setAreaSqft(int sqft);
    void setAddressLine(const std::string& addr);
    void setParkingAvailable(bool parking);

    // Polymorphic implementations
    virtual std::string getCategory() const override;
    virtual double calculateRent(int days) const override;
    virtual std::string toJson() const override;
};

#endif // HOUSE_H
