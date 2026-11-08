#include "Car.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Car::Car()
    : RentalItem(), m_brand(""), m_model(""), m_year(2023),
      m_fuelType("Petrol"), m_seatingCapacity(5),
      m_transmission("Manual"), m_registrationNumber(""),
      m_mileage("18 kmpl") {}

Car::Car(int id, const std::string& title, const std::string& description,
         double dailyRate, double securityDeposit, const std::string& location,
         const std::string& brand, const std::string& model, int year,
         const std::string& fuelType, int seatingCapacity,
         const std::string& transmission, const std::string& regNumber,
         const std::string& mileage, const std::string& status,
         const std::string& imageUrl, const std::string& createdAt)
    : RentalItem(id, title, description, dailyRate, securityDeposit, location, status, imageUrl, createdAt),
      m_brand(brand), m_model(model), m_year(year), m_fuelType(fuelType),
      m_seatingCapacity(seatingCapacity), m_transmission(transmission),
      m_registrationNumber(regNumber), m_mileage(mileage) {
    if (brand.empty() || model.empty()) {
        throw ValidationException("Car brand and model cannot be empty.");
    }
}

Car::~Car() {}

std::string Car::getBrand() const { return m_brand; }
std::string Car::getModel() const { return m_model; }
int Car::getYear() const { return m_year; }
std::string Car::getFuelType() const { return m_fuelType; }
int Car::getSeatingCapacity() const { return m_seatingCapacity; }
std::string Car::getTransmission() const { return m_transmission; }
std::string Car::getRegistrationNumber() const { return m_registrationNumber; }
std::string Car::getMileage() const { return m_mileage; }

void Car::setBrand(const std::string& brand) { m_brand = brand; }
void Car::setModel(const std::string& model) { m_model = model; }
void Car::setYear(int year) { m_year = year; }
void Car::setFuelType(const std::string& fuel) { m_fuelType = fuel; }
void Car::setSeatingCapacity(int seats) { m_seatingCapacity = seats; }
void Car::setTransmission(const std::string& trans) { m_transmission = trans; }
void Car::setRegistrationNumber(const std::string& reg) { m_registrationNumber = reg; }
void Car::setMileage(const std::string& mileage) { m_mileage = mileage; }

std::string Car::getCategory() const { return "Car"; }

// Polymorphic Rent Calculation with Tiered Long-Booking Discount
double Car::calculateRent(int days) const {
    if (days <= 0) return 0.0;
    double base = days * m_dailyRate;
    if (days >= 14) {
        return base * 0.85; // 15% discount for 2+ weeks
    } else if (days >= 7) {
        return base * 0.90; // 10% discount for 1+ week
    }
    return base;
}

std::string Car::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("title", m_title)
       << JsonHelper::kvString("category", "Car")
       << JsonHelper::kvString("description", m_description)
       << JsonHelper::kv("daily_rate", m_dailyRate)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kvString("location", m_location)
       << JsonHelper::kvString("status", m_status)
       << JsonHelper::kvString("image_url", m_imageUrl)
       << JsonHelper::kvString("brand", m_brand)
       << JsonHelper::kvString("model", m_model)
       << JsonHelper::kv("year", m_year)
       << JsonHelper::kvString("fuel_type", m_fuelType)
       << JsonHelper::kv("seating_capacity", m_seatingCapacity)
       << JsonHelper::kvString("transmission", m_transmission)
       << JsonHelper::kvString("registration_number", m_registrationNumber)
       << JsonHelper::kvString("mileage", m_mileage)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
