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
 * Implements salted SHA-256 password hashing with CSPRNG salt generation,
 * constant-time verification to prevent timing attacks, and legacy SHA-256
 * verification without hardcoded bypasses.
 * ============================================================================
 */
class PasswordHasher {
public:
    static std::string generateSalt(size_t byteCount = 16);
    static std::string hashPassword(const std::string& plainPassword);
    static std::string sha256(const std::string& input);
    static bool verifyPassword(const std::string& plainPassword, const std::string& storedHash);
    static bool constantTimeCompare(const std::string& a, const std::string& b);
};

#endif // PASSWORD_HASHER_H
