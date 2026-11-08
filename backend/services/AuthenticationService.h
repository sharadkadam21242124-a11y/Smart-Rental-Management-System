#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include "../database/UserRepository.h"
#include <string>
#include <map>
#include <mutex>
#include <memory>

struct SessionInfo {
    int userId;
    std::string email;
    std::string role;
    int customerOrAdminId;
    std::string fullName;
    long long loginTime;
};

/**
 * ============================================================================
 * OOP CONCEPT: Service Layer Pattern & Session Security
 * AuthenticationService manages login authentication, secure registration,
 * session token generation, role verification, and logout session teardown.
 * ============================================================================
 */
class AuthenticationService {
private:
    UserRepository& m_userRepo;
    std::map<std::string, SessionInfo> m_activeSessions;
    mutable std::mutex m_sessionMutex;

public:
    explicit AuthenticationService(UserRepository& repo);

    // Authentication Actions
    std::string registerCustomer(
        const std::string& email,
        const std::string& password,
        const std::string& fullName,
        const std::string& phone,
        const std::string& address,
        const std::string& idProof = "",
        const std::string& drivingLicense = ""
    );

    std::pair<std::string, SessionInfo> login(const std::string& email, const std::string& password);
    void logout(const std::string& token);

    bool updateProfile(int userId, const std::string& role, int accountId,
                       const std::string& fullName, const std::string& phone,
                       const std::string& address, const std::string& department = "");
    bool changePassword(int userId, const std::string& oldPassword, const std::string& newPassword);

    // Session Verification
    bool validateSession(const std::string& token, SessionInfo& outSession) const;
    bool requireRole(const std::string& token, const std::string& requiredRole, SessionInfo& outSession) const;

    // Helper token generator
    static std::string generateSessionToken(int userId, const std::string& email);
};

#endif // AUTHENTICATION_SERVICE_H
