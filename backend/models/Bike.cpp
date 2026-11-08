#include "Bike.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Bike::Bike()
    : RentalItem(), m_brand(""), m_model(""), m_year(2023),
      m_engineCapacityCc(150), m_bikeType("Commuter"),
      m_registrationNumber(""), m_helmetIncluded(true) {}

Bike::Bike(int id, const std::string& title, const std::string& description,
           double dailyRate, double securityDeposit, const std::string& location,
           const std::string& brand, const std::string& model, int year,
           int engineCapacityCc, const std::string& bikeType,
           const std::string& regNumber, bool helmetIncluded,
           const std::string& status, const std::string& imageUrl,
           const std::string& createdAt)
    : RentalItem(id, title, description, dailyRate, securityDeposit, location, status, imageUrl, createdAt),
      m_brand(brand), m_model(model), m_year(year),
      m_engineCapacityCc(engineCapacityCc), m_bikeType(bikeType),
      m_registrationNumber(regNumber), m_helmetIncluded(helmetIncluded) {
    if (brand.empty() || model.empty()) {
        throw ValidationException("Bike brand and model cannot be empty.");
    }
}

Bike::~Bike() {}

std::string Bike::getBrand() const { return m_brand; }
std::string Bike::getModel() const { return m_model; }
int Bike::getYear() const { return m_year; }
int Bike::getEngineCapacityCc() const { return m_engineCapacityCc; }
std::string Bike::getBikeType() const { return m_bikeType; }
std::string Bike::getRegistrationNumber() const { return m_registrationNumber; }
bool Bike::isHelmetIncluded() const { return m_helmetIncluded; }

void Bike::setBrand(const std::string& brand) { m_brand = brand; }
void Bike::setModel(const std::string& model) { m_model = model; }
void Bike::setYear(int year) { m_year = year; }
void Bike::setEngineCapacityCc(int cc) { m_engineCapacityCc = cc; }
void Bike::setBikeType(const std::string& type) { m_bikeType = type; }
void Bike::setRegistrationNumber(const std::string& reg) { m_registrationNumber = reg; }
void Bike::setHelmetIncluded(bool included) { m_helmetIncluded = included; }

std::string Bike::getCategory() const { return "Bike"; }

double Bike::calculateRent(int days) const {
    if (days <= 0) return 0.0;
    double base = days * m_dailyRate;
    if (days >= 10) {
        return base * 0.88; // 12% discount for 10+ days
    } else if (days >= 5) {
        return base * 0.95; // 5% discount for 5+ days
    }
    return base;
}

std::string Bike::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("title", m_title)
       << JsonHelper::kvString("category", "Bike")
       << JsonHelper::kvString("description", m_description)
       << JsonHelper::kv("daily_rate", m_dailyRate)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kvString("location", m_location)
       << JsonHelper::kvString("status", m_status)
       << JsonHelper::kvString("image_url", m_imageUrl)
       << JsonHelper::kvString("brand", m_brand)
       << JsonHelper::kvString("model", m_model)
       << JsonHelper::kv("year", m_year)
       << JsonHelper::kv("engine_capacity_cc", m_engineCapacityCc)
       << JsonHelper::kvString("bike_type", m_bikeType)
       << JsonHelper::kvString("registration_number", m_registrationNumber)
       << JsonHelper::kvBool("helmet_included", m_helmetIncluded)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
