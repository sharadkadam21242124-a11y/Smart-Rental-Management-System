#ifndef PASSWORD_HASHER_H
#define PASSWORD_HASHER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation & Cryptographic Security
 * Implements standard PBKDF2-HMAC-SHA256 with OWASP recommended 600,000 iterations
 * with CSPRNG salt generation via Windows CNG (BCryptDeriveKeyPBKDF2),
 * constant-time verification, and transparent forward/legacy work-factor migration.
 * ============================================================================
 */
class PasswordHasher {
public:
    // Current OWASP Recommendation for PBKDF2-HMAC-SHA256
    static constexpr uint32_t PBKDF2_RECOMMENDED_ITERATIONS = 600000;

    static std::string generateSalt(size_t byteCount = 16);
    static std::string pbkdf2(const std::string& password, const std::string& salt, uint32_t iterations = PBKDF2_RECOMMENDED_ITERATIONS);
    static std::string hashPassword(const std::string& plainPassword);
    static std::string sha256(const std::string& input);
    static bool verifyPassword(const std::string& plainPassword, const std::string& storedHash);
    static bool isLegacyHash(const std::string& storedHash);
    static bool constantTimeCompare(const std::string& a, const std::string& b);
};

#endif // PASSWORD_HASHER_H
