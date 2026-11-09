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
    long long loginTime; // Epoch seconds
    long long expiresAt; // Epoch seconds (loginTime + TTL)
};

/**
 * ============================================================================
 * OOP CONCEPT: Service Layer Pattern & Session Security
 * AuthenticationService manages login authentication, secure registration,
 * CSPRNG session token generation, role verification, session TTL expiration,
 * and logout session teardown.
 * ============================================================================
 */
class AuthenticationService {
private:
    UserRepository& m_userRepo;
    std::map<std::string, SessionInfo> m_activeSessions;
    mutable std::mutex m_sessionMutex;

    static constexpr long long SESSION_TTL_SECONDS = 86400; // 24-hour expiration

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
    void purgeExpiredSessions();

    bool updateProfile(int userId, const std::string& role, int accountId,
                       const std::string& fullName, const std::string& phone,
                       const std::string& address, const std::string& department = "");
    bool changePassword(int userId, const std::string& oldPassword, const std::string& newPassword);

    // Session Verification
    bool validateSession(const std::string& token, SessionInfo& outSession) const;
    bool requireRole(const std::string& token, const std::string& requiredRole, SessionInfo& outSession) const;

    // Cryptographically Secure Pseudo-Random Token Generator
    static std::string generateSessionToken();
};

#endif // AUTHENTICATION_SERVICE_H
