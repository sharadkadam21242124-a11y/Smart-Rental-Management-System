#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Single Inheritance, Encapsulation
 * Admin derives from User, representing managerial staff with department info.
 * ============================================================================
 */
class Admin : public User {
private:
    int m_adminId;
    std::string m_fullName;
    std::string m_phone;
    std::string m_department;

public:
    Admin();
    Admin(int userId, const std::string& email, const std::string& passwordHash,
          int adminId, const std::string& fullName, const std::string& phone = "",
          const std::string& department = "Operations", bool isActive = true,
          const std::string& createdAt = "");
    virtual ~Admin() override;

    int getAdminId() const;
    std::string getFullName() const;
    std::string getPhone() const;
    std::string getDepartment() const;

    void setAdminId(int id);
    void setFullName(const std::string& name);
    void setPhone(const std::string& phone);
    void setDepartment(const std::string& dept);

    virtual std::string getDisplaySummary() const override;
    virtual std::string toJson() const override;
};

#endif // ADMIN_H
