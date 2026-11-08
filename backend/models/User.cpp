#include "User.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

User::User()
    : m_id(0), m_email(""), m_passwordHash(""), m_role("customer"),
      m_isActive(true), m_createdAt("") {}

User::User(int id, const std::string& email, const std::string& passwordHash,
           const std::string& role, bool isActive, const std::string& createdAt)
    : m_id(id), m_email(email), m_passwordHash(passwordHash),
      m_role(role), m_isActive(isActive), m_createdAt(createdAt) {
    if (email.empty()) {
        throw ValidationException("User email cannot be empty.");
    }
}

User::User(const User& other)
    : m_id(other.m_id), m_email(other.m_email),
      m_passwordHash(other.m_passwordHash), m_role(other.m_role),
      m_isActive(other.m_isActive), m_createdAt(other.m_createdAt) {}

User::~User() {
    // Virtual destructor body ensures clean destruction of derived objects
}

int User::getId() const { return m_id; }
std::string User::getEmail() const { return m_email; }
std::string User::getPasswordHash() const { return m_passwordHash; }
std::string User::getRole() const { return m_role; }
bool User::isActive() const { return m_isActive; }
std::string User::getCreatedAt() const { return m_createdAt; }

void User::setId(int id) { m_id = id; }

void User::setEmail(const std::string& email) {
    if (email.find('@') == std::string::npos) {
        throw ValidationException("Invalid email format: " + email);
    }
    m_email = email;
}

void User::setPasswordHash(const std::string& hash) {
    if (hash.empty()) {
        throw ValidationException("Password hash cannot be empty.");
    }
    m_passwordHash = hash;
}

void User::setRole(const std::string& role) {
    if (role != "customer" && role != "admin") {
        throw ValidationException("Role must be 'customer' or 'admin'");
    }
    m_role = role;
}

void User::setActive(bool active) { m_isActive = active; }
void User::setCreatedAt(const std::string& createdAt) { m_createdAt = createdAt; }

std::string User::getDisplaySummary() const {
    std::ostringstream ss;
    ss << "User #" << m_id << " (" << m_email << ") [" << m_role << "]";
    return ss.str();
}

std::string User::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("email", m_email)
       << JsonHelper::kvString("role", m_role)
       << JsonHelper::kvBool("is_active", m_isActive)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
