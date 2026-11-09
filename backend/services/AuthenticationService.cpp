#include "AuthenticationService.h"
#include "../utils/PasswordHasher.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"
#include <chrono>
#include <sstream>

AuthenticationService::AuthenticationService(UserRepository& repo) : m_userRepo(repo) {}

std::string AuthenticationService::generateSessionToken() {
    // 256-bit cryptographically secure random session token (64 hex characters)
    return "SRMS-" + PasswordHasher::generateSalt(32);
}

std::string AuthenticationService::registerCustomer(
    const std::string& email,
    const std::string& password,
    const std::string& fullName,
    const std::string& phone,
    const std::string& address,
    const std::string& idProof,
    const std::string& drivingLicense
) {
    if (password.length() < 6) {
        throw ValidationException("Password must be at least 6 characters long.");
    }
    if (fullName.empty() || phone.empty() || address.empty()) {
        throw ValidationException("Name, phone, and address are mandatory fields.");
    }

    std::string passwordHash = PasswordHasher::hashPassword(password);
    auto customer = m_userRepo.createCustomer(email, passwordHash, fullName, phone, address, idProof, drivingLicense);

    std::string token = generateSessionToken();

    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    SessionInfo session;
    session.userId = customer->getId();
    session.email = customer->getEmail();
    session.role = "customer";
    session.customerOrAdminId = customer->getCustomerId();
    session.fullName = customer->getFullName();
    session.loginTime = now;
    session.expiresAt = now + SESSION_TTL_SECONDS;

    {
        std::lock_guard<std::mutex> lock(m_sessionMutex);
        m_activeSessions[token] = session;
    }

    Logger::info("New customer registered: " + email + " [User #" + std::to_string(customer->getId()) + "]");
    return token;
}

std::pair<std::string, SessionInfo> AuthenticationService::login(const std::string& email, const std::string& password) {
    auto user = m_userRepo.findByEmail(email);
    if (!user) {
        throw AuthenticationException("Invalid email or password.");
    }
    if (!user->isActive()) {
        throw AuthenticationException("Account has been suspended. Please contact the administrator.");
    }

    if (!PasswordHasher::verifyPassword(password, user->getPasswordHash())) {
        throw AuthenticationException("Invalid email or password.");
    }

    std::string token = generateSessionToken();

    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    SessionInfo session;
    session.userId = user->getId();
    session.email = user->getEmail();
    session.role = user->getRole();
    session.loginTime = now;
    session.expiresAt = now + SESSION_TTL_SECONDS;

    if (user->getRole() == "customer") {
        auto cust = m_userRepo.findCustomerByUserId(user->getId());
        if (cust) {
            session.customerOrAdminId = cust->getCustomerId();
            session.fullName = cust->getFullName();
        }
    } else {
        auto adm = m_userRepo.findAdminByUserId(user->getId());
        if (adm) {
            session.customerOrAdminId = adm->getAdminId();
            session.fullName = adm->getFullName();
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_sessionMutex);
        m_activeSessions[token] = session;
    }

    Logger::info("User logged in: " + email + " (" + user->getRole() + ")");
    return {token, session};
}

void AuthenticationService::logout(const std::string& token) {
    std::lock_guard<std::mutex> lock(m_sessionMutex);
    m_activeSessions.erase(token);
    Logger::info("User session logged out.");
}

void AuthenticationService::purgeExpiredSessions() {
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    std::lock_guard<std::mutex> lock(m_sessionMutex);
    for (auto it = m_activeSessions.begin(); it != m_activeSessions.end();) {
        if (now > it->second.expiresAt) {
            it = m_activeSessions.erase(it);
        } else {
            ++it;
        }
    }
}

bool AuthenticationService::validateSession(const std::string& token, SessionInfo& outSession) const {
    if (token.empty()) return false;

    std::lock_guard<std::mutex> lock(m_sessionMutex);
    auto it = m_activeSessions.find(token);
    if (it != m_activeSessions.end()) {
        auto now = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();

        // Enforce 24-hour expiration TTL
        if (now > it->second.expiresAt) {
            return false;
        }

        outSession = it->second;
        return true;
    }
    return false;
}

bool AuthenticationService::requireRole(const std::string& token, const std::string& requiredRole, SessionInfo& outSession) const {
    if (!validateSession(token, outSession)) return false;
    return (outSession.role == requiredRole);
}

bool AuthenticationService::updateProfile(int userId, const std::string& role, int accountId,
                                         const std::string& fullName, const std::string& phone,
                                         const std::string& address, const std::string& department) {
    if (role == "admin") {
        return m_userRepo.updateAdminProfile(accountId, fullName, phone, department);
    } else {
        return m_userRepo.updateCustomerProfile(accountId, fullName, phone, address);
    }
}

bool AuthenticationService::changePassword(int userId, const std::string& oldPassword, const std::string& newPassword) {
    if (newPassword.length() < 6) {
        throw ValidationException("New password must be at least 6 characters long.");
    }

    auto user = m_userRepo.findById(userId);
    if (!user) {
        throw AuthenticationException("User account not found.");
    }

    if (!PasswordHasher::verifyPassword(oldPassword, user->getPasswordHash())) {
        throw AuthenticationException("Incorrect current password.");
    }

    std::string newHash = PasswordHasher::hashPassword(newPassword);
    return m_userRepo.updatePassword(userId, newHash);
}
