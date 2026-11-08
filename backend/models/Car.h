#ifndef CAR_H
#define CAR_H

#include "RentalItem.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Hierarchical Inheritance & Polymorphism
 * Car derives from RentalItem, implementing custom rent calculation
 * and specialized automotive specifications.
 * ============================================================================
 */
class Car : public RentalItem {
private:
    std::string m_brand;
    std::string m_model;
    int m_year;
    std::string m_fuelType; // Petrol, Diesel, Electric, Hybrid, CNG
    int m_seatingCapacity;
    std::string m_transmission; // Manual, Automatic
    std::string m_registrationNumber;
    std::string m_mileage;

public:
    Car();
    Car(int id, const std::string& title, const std::string& description,
        double dailyRate, double securityDeposit, const std::string& location,
        const std::string& brand, const std::string& model, int year,
        const std::string& fuelType, int seatingCapacity,
        const std::string& transmission, const std::string& regNumber,
        const std::string& mileage = "18 kmpl",
        const std::string& status = "available",
        const std::string& imageUrl = "",
        const std::string& createdAt = "");
    virtual ~Car() override;

    // Getters
    std::string getBrand() const;
    std::string getModel() const;
    int getYear() const;
    std::string getFuelType() const;
    int getSeatingCapacity() const;
    std::string getTransmission() const;
    std::string getRegistrationNumber() const;
    std::string getMileage() const;

    // Setters
    void setBrand(const std::string& brand);
    void setModel(const std::string& model);
    void setYear(int year);
    void setFuelType(const std::string& fuel);
    void setSeatingCapacity(int seats);
    void setTransmission(const std::string& trans);
    void setRegistrationNumber(const std::string& reg);
    void setMileage(const std::string& mileage);

    // Polymorphic implementations
    virtual std::string getCategory() const override;
    virtual double calculateRent(int days) const override;
    virtual std::string toJson() const override;
};

#endif // CAR_H
