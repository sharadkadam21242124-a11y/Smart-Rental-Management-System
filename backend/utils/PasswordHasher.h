#ifndef PASSWORD_HASHER_H
#define PASSWORD_HASHER_H

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

/**
 * ============================================================================
 * OOP CONCEPT: Encapsulation & Secure Hashing
 * Implements standard SHA-256 algorithm with optional salt support
 * to prevent storing plain-text passwords in the database.
 * ============================================================================
 */
class PasswordHasher {
public:
    static std::string sha256(const std::string& input);
    static bool verifyPassword(const std::string& plainPassword, const std::string& storedHash);
};

#endif // PASSWORD_HASHER_H
