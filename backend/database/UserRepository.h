#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "../models/User.h"
#include "../models/Customer.h"
#include "../models/Admin.h"
#include "Database.h"
#include <memory>
#include <vector>
#include <optional>

/**
 * ============================================================================
 * OOP CONCEPT: Repository Design Pattern & Dependency Inversion
 * UserRepository abstracts data access operations for Users, Customers,
 * and Admins from the MySQL database tables.
 * ============================================================================
 */
class UserRepository {
private:
    Database& m_db;

public:
    explicit UserRepository(Database& db = Database::getInstance());

    std::shared_ptr<User> findByEmail(const std::string& email);
    std::shared_ptr<User> findById(int id);
    std::shared_ptr<Customer> findCustomerByUserId(int userId);
    std::shared_ptr<Customer> findCustomerById(int customerId);
    std::shared_ptr<Admin> findAdminByUserId(int userId);

    // Customer Registration Transaction
    std::shared_ptr<Customer> createCustomer(
        const std::string& email,
        const std::string& passwordHash,
        const std::string& fullName,
        const std::string& phone,
        const std::string& address,
        const std::string& idProof = "",
        const std::string& drivingLicense = ""
    );

    std::vector<std::shared_ptr<Customer>> getAllCustomers();
    bool setCustomerActiveStatus(int userId, bool isActive);
    bool updateCustomerProfile(int customerId, const std::string& fullName,
                               const std::string& phone, const std::string& address);
    bool updateAdminProfile(int adminId, const std::string& fullName,
                            const std::string& phone, const std::string& department);
    bool updatePassword(int userId, const std::string& newPasswordHash);
    int getTotalCustomerCount();
};

#endif // USER_REPOSITORY_H
