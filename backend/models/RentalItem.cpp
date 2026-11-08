#include "RentalItem.h"
#include "../utils/Exceptions.h"
#include <sstream>

// Initialize static member variable
int RentalItem::s_activeItemCount = 0;

RentalItem::RentalItem()
    : m_id(0), m_title(""), m_description(""), m_dailyRate(0.0),
      m_securityDeposit(0.0), m_location(""), m_status("available"),
      m_imageUrl(""), m_createdAt("") {
    ++s_activeItemCount;
}

RentalItem::RentalItem(int id, const std::string& title, const std::string& description,
                       double dailyRate, double securityDeposit, const std::string& location,
                       const std::string& status, const std::string& imageUrl,
                       const std::string& createdAt)
    : m_id(id), m_title(title), m_description(description),
      m_dailyRate(dailyRate), m_securityDeposit(securityDeposit),
      m_location(location), m_status(status), m_imageUrl(imageUrl),
      m_createdAt(createdAt) {
    if (title.empty()) throw ValidationException("Rental item title cannot be empty.");
    if (dailyRate < 0.0) throw ValidationException("Daily rental rate cannot be negative.");
    if (securityDeposit < 0.0) throw ValidationException("Security deposit cannot be negative.");
    ++s_activeItemCount;
}

RentalItem::RentalItem(const RentalItem& other)
    : m_id(other.m_id), m_title(other.m_title), m_description(other.m_description),
      m_dailyRate(other.m_dailyRate), m_securityDeposit(other.m_securityDeposit),
      m_location(other.m_location), m_status(other.m_status),
      m_imageUrl(other.m_imageUrl), m_createdAt(other.m_createdAt) {
    ++s_activeItemCount;
}

RentalItem::~RentalItem() {
    --s_activeItemCount;
}

int RentalItem::getId() const { return m_id; }
std::string RentalItem::getTitle() const { return m_title; }
std::string RentalItem::getDescription() const { return m_description; }
double RentalItem::getDailyRate() const { return m_dailyRate; }
double RentalItem::getSecurityDeposit() const { return m_securityDeposit; }
std::string RentalItem::getLocation() const { return m_location; }
std::string RentalItem::getStatus() const { return m_status; }
std::string RentalItem::getImageUrl() const { return m_imageUrl; }
std::string RentalItem::getCreatedAt() const { return m_createdAt; }

void RentalItem::setId(int id) { m_id = id; }
void RentalItem::setTitle(const std::string& title) {
    if (title.empty()) throw ValidationException("Title cannot be empty.");
    m_title = title;
}
void RentalItem::setDescription(const std::string& desc) { m_description = desc; }
void RentalItem::setDailyRate(double rate) {
    if (rate < 0.0) throw ValidationException("Daily rate cannot be negative.");
    m_dailyRate = rate;
}
void RentalItem::setSecurityDeposit(double deposit) {
    if (deposit < 0.0) throw ValidationException("Deposit cannot be negative.");
    m_securityDeposit = deposit;
}
void RentalItem::setLocation(const std::string& loc) { m_location = loc; }
void RentalItem::setStatus(const std::string& status) { m_status = status; }
void RentalItem::setImageUrl(const std::string& url) { m_imageUrl = url; }

int RentalItem::getActiveItemCount() { return s_activeItemCount; }

const std::vector<std::string>& RentalItem::getSupportedCategories() {
    static const std::vector<std::string> categories = {"Car", "Bike", "House", "Furniture"};
    return categories;
}

// Function Overload: calculateRent with discount percentage
double RentalItem::calculateRent(int days, double discountPercentage) const {
    double baseRent = this->calculateRent(days);
    if (discountPercentage > 0.0) {
        double discount = baseRent * (discountPercentage / 100.0);
        return baseRent - discount;
    }
    return baseRent;
}

double RentalItem::calculateTotalPayable(int days, double discountPercentage) const {
    return calculateRent(days, discountPercentage) + m_securityDeposit;
}

// Operator Overloading
bool RentalItem::operator<(const RentalItem& other) const {
    return this->m_dailyRate < other.m_dailyRate;
}

bool RentalItem::operator==(const RentalItem& other) const {
    return this->m_id == other.m_id;
}

// Friend Functions
std::ostream& operator<<(std::ostream& os, const RentalItem& item) {
    os << "[" << item.getCategory() << " #" << item.m_id << "] "
       << item.m_title << " | Rate: ₹" << item.m_dailyRate << "/day"
       << " | Deposit: ₹" << item.m_securityDeposit
       << " | Location: " << item.m_location
       << " | Status: " << item.m_status;
    return os;
}

bool compareRentalValue(const RentalItem& a, const RentalItem& b) {
    // Compares value-for-money metric
    return a.m_dailyRate < b.m_dailyRate;
}
