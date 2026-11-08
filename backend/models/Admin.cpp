#include "Admin.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Admin::Admin()
    : User(), m_adminId(0), m_fullName(""), m_phone(""), m_department("Operations") {
    m_role = "admin";
}

Admin::Admin(int userId, const std::string& email, const std::string& passwordHash,
             int adminId, const std::string& fullName, const std::string& phone,
             const std::string& department, bool isActive, const std::string& createdAt)
    : User(userId, email, passwordHash, "admin", isActive, createdAt),
      m_adminId(adminId), m_fullName(fullName), m_phone(phone), m_department(department) {
    if (fullName.empty()) {
        throw ValidationException("Admin full name cannot be empty.");
    }
}

Admin::~Admin() {}

int Admin::getAdminId() const { return m_adminId; }
std::string Admin::getFullName() const { return m_fullName; }
std::string Admin::getPhone() const { return m_phone; }
std::string Admin::getDepartment() const { return m_department; }

void Admin::setAdminId(int id) { m_adminId = id; }
void Admin::setFullName(const std::string& name) {
    if (name.empty()) throw ValidationException("Admin name cannot be empty.");
    m_fullName = name;
}
void Admin::setPhone(const std::string& phone) { m_phone = phone; }
void Admin::setDepartment(const std::string& dept) { m_department = dept; }

std::string Admin::getDisplaySummary() const {
    std::ostringstream ss;
    ss << "Admin #" << m_adminId << " " << m_fullName << " (" << m_department << ")";
    return ss.str();
}

std::string Admin::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("user_id", m_id)
       << JsonHelper::kv("admin_id", m_adminId)
       << JsonHelper::kvString("email", m_email)
       << JsonHelper::kvString("role", m_role)
       << JsonHelper::kvString("full_name", m_fullName)
       << JsonHelper::kvString("phone", m_phone)
       << JsonHelper::kvString("department", m_department)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
