#include "Customer.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Customer::Customer()
    : User(), m_customerId(0), m_fullName(""), m_phone(""),
      m_address(""), m_idProofNumber(""), m_drivingLicenseNumber("") {
    m_role = "customer";
}

Customer::Customer(int userId, const std::string& email, const std::string& passwordHash,
                   int customerId, const std::string& fullName, const std::string& phone,
                   const std::string& address, const std::string& idProof,
                   const std::string& drivingLicense, bool isActive,
                   const std::string& createdAt)
    : User(userId, email, passwordHash, "customer", isActive, createdAt),
      m_customerId(customerId), m_fullName(fullName), m_phone(phone),
      m_address(address), m_idProofNumber(idProof),
      m_drivingLicenseNumber(drivingLicense) {
    if (fullName.empty()) {
        throw ValidationException("Customer name cannot be empty.");
    }
}

Customer::Customer(const Customer& other)
    : User(other), m_customerId(other.m_customerId),
      m_fullName(other.m_fullName), m_phone(other.m_phone),
      m_address(other.m_address), m_idProofNumber(other.m_idProofNumber),
      m_drivingLicenseNumber(other.m_drivingLicenseNumber) {}

Customer::~Customer() {}

int Customer::getCustomerId() const { return m_customerId; }
std::string Customer::getFullName() const { return m_fullName; }
std::string Customer::getPhone() const { return m_phone; }
std::string Customer::getAddress() const { return m_address; }
std::string Customer::getIdProofNumber() const { return m_idProofNumber; }
std::string Customer::getDrivingLicenseNumber() const { return m_drivingLicenseNumber; }

void Customer::setCustomerId(int id) { m_customerId = id; }

void Customer::setFullName(const std::string& name) {
    if (name.empty()) throw ValidationException("Full name cannot be empty.");
    m_fullName = name;
}

void Customer::setPhone(const std::string& phone) { m_phone = phone; }
void Customer::setAddress(const std::string& address) { m_address = address; }
void Customer::setIdProofNumber(const std::string& proof) { m_idProofNumber = proof; }
void Customer::setDrivingLicenseNumber(const std::string& dl) { m_drivingLicenseNumber = dl; }

std::string Customer::getDisplaySummary() const {
    std::ostringstream ss;
    ss << "Customer #" << m_customerId << " " << m_fullName
       << " (Phone: " << m_phone << ", Email: " << m_email << ")";
    return ss.str();
}

std::string Customer::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("user_id", m_id)
       << JsonHelper::kv("customer_id", m_customerId)
       << JsonHelper::kvString("email", m_email)
       << JsonHelper::kvString("role", m_role)
       << JsonHelper::kvBool("is_active", m_isActive)
       << JsonHelper::kvString("full_name", m_fullName)
       << JsonHelper::kvString("phone", m_phone)
       << JsonHelper::kvString("address", m_address)
       << JsonHelper::kvString("id_proof_number", m_idProofNumber)
       << JsonHelper::kvString("driving_license_number", m_drivingLicenseNumber)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
