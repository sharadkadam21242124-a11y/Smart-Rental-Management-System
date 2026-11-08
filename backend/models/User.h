#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

/**
 * ============================================================================
 * OOP CONCEPT: Base Class, Encapsulation, Virtual Destructor, Const Methods
 * 'User' represents the core authentication identity for the system.
 * Serves as the base class in the Single/Multilevel user hierarchy.
 * ============================================================================
 */
class User {
protected:
    int m_id;
    std::string m_email;
    std::string m_passwordHash;
    std::string m_role; // "customer" or "admin"
    bool m_isActive;
    std::string m_createdAt;

public:
    // 1. Default Constructor
    User();

    // 2. Parameterized Constructor
    User(int id, const std::string& email, const std::string& passwordHash,
         const std::string& role = "customer", bool isActive = true,
         const std::string& createdAt = "");

    // 3. Copy Constructor (Demonstrating explicit copy semantics)
    User(const User& other);

    // 4. Virtual Destructor (Crucial for polymorphic base classes)
    virtual ~User();

    // 5. Encapsulation: Getters (Const Member Functions)
    int getId() const;
    std::string getEmail() const;
    std::string getPasswordHash() const;
    std::string getRole() const;
    bool isActive() const;
    std::string getCreatedAt() const;

    // 6. Encapsulation: Setters with state validation
    void setId(int id);
    void setEmail(const std::string& email);
    void setPasswordHash(const std::string& hash);
    void setRole(const std::string& role);
    void setActive(bool active);
    void setCreatedAt(const std::string& createdAt);

    // 7. Virtual function for polymorphic identity display
    virtual std::string getDisplaySummary() const;

    // 8. Virtual function to export JSON representation
    virtual std::string toJson() const;
};

#endif // USER_H
