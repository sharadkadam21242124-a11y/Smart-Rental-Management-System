#include "UserRepository.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"
#include <sstream>

UserRepository::UserRepository(Database& db) : m_db(db) {}

std::shared_ptr<User> UserRepository::findByEmail(const std::string& email) {
    std::string safeEmail = m_db.escapeString(email);
    std::ostringstream ss;
    ss << "SELECT id, email, password_hash, role, is_active, created_at "
       << "FROM users WHERE email = '" << safeEmail << "' LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const QueryRow& row = res[0];
    return std::make_shared<User>(
        row.getInt("id"),
        row.getString("email"),
        row.getString("password_hash"),
        row.getString("role"),
        row.getBool("is_active"),
        row.getString("created_at")
    );
}

std::shared_ptr<User> UserRepository::findById(int id) {
    std::ostringstream ss;
    ss << "SELECT id, email, password_hash, role, is_active, created_at "
       << "FROM users WHERE id = " << id << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const QueryRow& row = res[0];
    return std::make_shared<User>(
        row.getInt("id"),
        row.getString("email"),
        row.getString("password_hash"),
        row.getString("role"),
        row.getBool("is_active"),
        row.getString("created_at")
    );
}

std::shared_ptr<Customer> UserRepository::findCustomerByUserId(int userId) {
    std::ostringstream ss;
    ss << "SELECT u.id AS user_id, u.email, u.password_hash, u.is_active, u.created_at, "
       << "c.id AS customer_id, c.full_name, c.phone, c.address, c.id_proof_number, c.driving_license_number "
       << "FROM users u "
       << "JOIN customers c ON u.id = c.user_id "
       << "WHERE u.id = " << userId << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const QueryRow& row = res[0];
    return std::make_shared<Customer>(
        row.getInt("user_id"),
        row.getString("email"),
        row.getString("password_hash"),
        row.getInt("customer_id"),
        row.getString("full_name"),
        row.getString("phone"),
        row.getString("address"),
        row.getString("id_proof_number"),
        row.getString("driving_license_number"),
        row.getBool("is_active"),
        row.getString("created_at")
    );
}

std::shared_ptr<Customer> UserRepository::findCustomerById(int customerId) {
    std::ostringstream ss;
    ss << "SELECT u.id AS user_id, u.email, u.password_hash, u.is_active, u.created_at, "
       << "c.id AS customer_id, c.full_name, c.phone, c.address, c.id_proof_number, c.driving_license_number "
       << "FROM users u "
       << "JOIN customers c ON u.id = c.user_id "
       << "WHERE c.id = " << customerId << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const QueryRow& row = res[0];
    return std::make_shared<Customer>(
        row.getInt("user_id"),
        row.getString("email"),
        row.getString("password_hash"),
        row.getInt("customer_id"),
        row.getString("full_name"),
        row.getString("phone"),
        row.getString("address"),
        row.getString("id_proof_number"),
        row.getString("driving_license_number"),
        row.getBool("is_active"),
        row.getString("created_at")
    );
}

std::shared_ptr<Admin> UserRepository::findAdminByUserId(int userId) {
    std::ostringstream ss;
    ss << "SELECT u.id AS user_id, u.email, u.password_hash, u.is_active, u.created_at, "
       << "a.id AS admin_id, a.full_name, a.phone, a.department "
       << "FROM users u "
       << "JOIN admins a ON u.id = a.user_id "
       << "WHERE u.id = " << userId << " LIMIT 1;";

    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;

    const QueryRow& row = res[0];
    return std::make_shared<Admin>(
        row.getInt("user_id"),
        row.getString("email"),
        row.getString("password_hash"),
        row.getInt("admin_id"),
        row.getString("full_name"),
        row.getString("phone"),
        row.getString("department"),
        row.getBool("is_active"),
        row.getString("created_at")
    );
}

std::shared_ptr<Customer> UserRepository::createCustomer(
    const std::string& email,
    const std::string& passwordHash,
    const std::string& fullName,
    const std::string& phone,
    const std::string& address,
    const std::string& idProof,
    const std::string& drivingLicense
) {
    // Check duplicate email
    if (findByEmail(email) != nullptr) {
        throw ValidationException("An account with email '" + email + "' already exists.");
    }

    // RAII Transaction Guard ensures atomicity of user + customer table inserts
    TransactionGuard guard(m_db);

    std::string safeEmail = m_db.escapeString(email);
    std::string safeName = m_db.escapeString(fullName);
    std::string safePhone = m_db.escapeString(phone);
    std::string safeAddress = m_db.escapeString(address);
    std::string safeIdProof = m_db.escapeString(idProof);
    std::string safeDl = m_db.escapeString(drivingLicense);

    std::ostringstream ssUser;
    ssUser << "INSERT INTO users (email, password_hash, role, is_active) "
           << "VALUES ('" << safeEmail << "', '" << passwordHash << "', 'customer', 1);";
    m_db.executeUpdate(ssUser.str());
    int userId = static_cast<int>(m_db.getLastInsertId());

    std::ostringstream ssCust;
    ssCust << "INSERT INTO customers (user_id, full_name, phone, address, id_proof_number, driving_license_number) "
           << "VALUES (" << userId << ", '" << safeName << "', '" << safePhone << "', '"
           << safeAddress << "', '" << safeIdProof << "', '" << safeDl << "');";
    m_db.executeUpdate(ssCust.str());
    int customerId = static_cast<int>(m_db.getLastInsertId());

    guard.commit();

    return std::make_shared<Customer>(
        userId, email, passwordHash, customerId, fullName, phone, address, idProof, drivingLicense, true
    );
}

std::vector<std::shared_ptr<Customer>> UserRepository::getAllCustomers() {
    std::vector<std::shared_ptr<Customer>> customers;
    std::string sql = "SELECT u.id AS user_id, u.email, u.password_hash, u.is_active, u.created_at, "
                      "c.id AS customer_id, c.full_name, c.phone, c.address, c.id_proof_number, c.driving_license_number "
                      "FROM users u "
                      "JOIN customers c ON u.id = c.user_id "
                      "ORDER BY c.id DESC;";

    QueryResult res = m_db.executeQuery(sql);
    for (const auto& row : res.getRows()) {
        customers.push_back(std::make_shared<Customer>(
            row.getInt("user_id"),
            row.getString("email"),
            row.getString("password_hash"),
            row.getInt("customer_id"),
            row.getString("full_name"),
            row.getString("phone"),
            row.getString("address"),
            row.getString("id_proof_number"),
            row.getString("driving_license_number"),
            row.getBool("is_active"),
            row.getString("created_at")
        ));
    }
    return customers;
}

bool UserRepository::setCustomerActiveStatus(int userId, bool isActive) {
    std::ostringstream ss;
    ss << "UPDATE users SET is_active = " << (isActive ? 1 : 0)
       << " WHERE id = " << userId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool UserRepository::updateCustomerProfile(int customerId, const std::string& fullName,
                                          const std::string& phone, const std::string& address) {
    std::ostringstream ss;
    ss << "UPDATE customers SET full_name = '" << m_db.escapeString(fullName)
       << "', phone = '" << m_db.escapeString(phone)
       << "', address = '" << m_db.escapeString(address)
       << "' WHERE id = " << customerId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool UserRepository::updateAdminProfile(int adminId, const std::string& fullName,
                                       const std::string& phone, const std::string& department) {
    std::ostringstream ss;
    ss << "UPDATE admins SET full_name = '" << m_db.escapeString(fullName)
       << "', phone = '" << m_db.escapeString(phone)
       << "', department = '" << m_db.escapeString(department)
       << "' WHERE id = " << adminId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool UserRepository::updatePassword(int userId, const std::string& newPasswordHash) {
    std::ostringstream ss;
    ss << "UPDATE users SET password_hash = '" << newPasswordHash
       << "' WHERE id = " << userId << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

int UserRepository::getTotalCustomerCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM customers;");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}
