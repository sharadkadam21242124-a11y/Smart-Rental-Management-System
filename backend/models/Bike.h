#ifndef BIKE_H
#define BIKE_H

#include "RentalItem.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Hierarchical Inheritance & Polymorphism
 * Bike derives from RentalItem, specialized for two-wheelers and scooters.
 * ============================================================================
 */
class Bike : public RentalItem {
private:
    std::string m_brand;
    std::string m_model;
    int m_year;
    int m_engineCapacityCc;
    std::string m_bikeType; // Commuter, Sports, Cruiser, Scooter, Adventure
    std::string m_registrationNumber;
    bool m_helmetIncluded;

public:
    Bike();
    Bike(int id, const std::string& title, const std::string& description,
         double dailyRate, double securityDeposit, const std::string& location,
         const std::string& brand, const std::string& model, int year,
         int engineCapacityCc, const std::string& bikeType,
         const std::string& regNumber, bool helmetIncluded = true,
         const std::string& status = "available",
         const std::string& imageUrl = "",
         const std::string& createdAt = "");
    virtual ~Bike() override;

    // Getters
    std::string getBrand() const;
    std::string getModel() const;
    int getYear() const;
    int getEngineCapacityCc() const;
    std::string getBikeType() const;
    std::string getRegistrationNumber() const;
    bool isHelmetIncluded() const;

    // Setters
    void setBrand(const std::string& brand);
    void setModel(const std::string& model);
    void setYear(int year);
    void setEngineCapacityCc(int cc);
    void setBikeType(const std::string& type);
    void setRegistrationNumber(const std::string& reg);
    void setHelmetIncluded(bool included);

    // Polymorphic implementations
    virtual std::string getCategory() const override;
    virtual double calculateRent(int days) const override;
    virtual std::string toJson() const override;
};

#endif // BIKE_H
