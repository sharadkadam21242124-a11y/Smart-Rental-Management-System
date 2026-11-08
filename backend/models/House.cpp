#include "House.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

House::House()
    : RentalItem(), m_propertyType("Apartment"), m_bedrooms(1),
      m_bathrooms(1), m_furnishing("Furnished"), m_areaSqft(800),
      m_addressLine(""), m_parkingAvailable(true) {}

House::House(int id, const std::string& title, const std::string& description,
             double dailyRate, double securityDeposit, const std::string& location,
             const std::string& propertyType, int bedrooms, int bathrooms,
             const std::string& furnishing, int areaSqft,
             const std::string& addressLine, bool parkingAvailable,
             const std::string& status, const std::string& imageUrl,
             const std::string& createdAt)
    : RentalItem(id, title, description, dailyRate, securityDeposit, location, status, imageUrl, createdAt),
      m_propertyType(propertyType), m_bedrooms(bedrooms),
      m_bathrooms(bathrooms), m_furnishing(furnishing),
      m_areaSqft(areaSqft), m_addressLine(addressLine),
      m_parkingAvailable(parkingAvailable) {
    if (bedrooms <= 0 || bathrooms <= 0) {
        throw ValidationException("Bedrooms and bathrooms must be at least 1.");
    }
}

House::~House() {}

std::string House::getPropertyType() const { return m_propertyType; }
int House::getBedrooms() const { return m_bedrooms; }
int House::getBathrooms() const { return m_bathrooms; }
std::string House::getFurnishing() const { return m_furnishing; }
int House::getAreaSqft() const { return m_areaSqft; }
std::string House::getAddressLine() const { return m_addressLine; }
bool House::isParkingAvailable() const { return m_parkingAvailable; }

void House::setPropertyType(const std::string& type) { m_propertyType = type; }
void House::setBedrooms(int beds) { m_bedrooms = beds; }
void House::setBathrooms(int baths) { m_bathrooms = baths; }
void House::setFurnishing(const std::string& furnishing) { m_furnishing = furnishing; }
void House::setAreaSqft(int sqft) { m_areaSqft = sqft; }
void House::setAddressLine(const std::string& addr) { m_addressLine = addr; }
void House::setParkingAvailable(bool parking) { m_parkingAvailable = parking; }

std::string House::getCategory() const { return "House"; }

double House::calculateRent(int days) const {
    if (days <= 0) return 0.0;
    double base = days * m_dailyRate;
    if (days >= 30) {
        return base * 0.80; // 20% discount for monthly leases
    } else if (days >= 14) {
        return base * 0.90; // 10% discount for fortnightly stays
    }
    return base;
}

std::string House::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("title", m_title)
       << JsonHelper::kvString("category", "House")
       << JsonHelper::kvString("description", m_description)
       << JsonHelper::kv("daily_rate", m_dailyRate)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kvString("location", m_location)
       << JsonHelper::kvString("status", m_status)
       << JsonHelper::kvString("image_url", m_imageUrl)
       << JsonHelper::kvString("property_type", m_propertyType)
       << JsonHelper::kv("bedrooms", m_bedrooms)
       << JsonHelper::kv("bathrooms", m_bathrooms)
       << JsonHelper::kvString("furnishing", m_furnishing)
       << JsonHelper::kv("area_sqft", m_areaSqft)
       << JsonHelper::kvString("address_line", m_addressLine)
       << JsonHelper::kvBool("parking_available", m_parkingAvailable)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
