#include "PasswordHasher.h"
#include <cstring>
#include <random>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#endif

namespace {
    // Standard SHA-256 constants and bitwise transformations
    inline uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
    inline uint32_t choose(uint32_t e, uint32_t f, uint32_t g) { return (e & f) ^ (~e & g); }
    inline uint32_t majority(uint32_t a, uint32_t b, uint32_t c) { return (a & b) ^ (a & c) ^ (b & c); }
    inline uint32_t sig0(uint32_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }
    inline uint32_t sig1(uint32_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }
    inline uint32_t theta0(uint32_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3); }
    inline uint32_t theta1(uint32_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10); }

    const uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
}

std::string PasswordHasher::sha256(const std::string& input) {
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    std::vector<uint8_t> msg(input.begin(), input.end());
    uint64_t bitLen = msg.size() * 8;

    msg.push_back(0x80);
    while ((msg.size() % 64) != 56) {
        msg.push_back(0x00);
    }

    for (int i = 7; i >= 0; --i) {
        msg.push_back(static_cast<uint8_t>((bitLen >> (i * 8)) & 0xff));
    }

    for (size_t chunk = 0; chunk < msg.size(); chunk += 64) {
        uint32_t W[64];
        for (int t = 0; t < 16; ++t) {
            W[t] = (msg[chunk + t * 4] << 24) |
                   (msg[chunk + t * 4 + 1] << 16) |
                   (msg[chunk + t * 4 + 2] << 8) |
                   (msg[chunk + t * 4 + 3]);
        }
        for (int t = 16; t < 64; ++t) {
            W[t] = theta1(W[t - 2]) + W[t - 7] + theta0(W[t - 15]) + W[t - 16];
        }

        uint32_t a = H[0], b = H[1], c = H[2], d = H[3];
        uint32_t e = H[4], f = H[5], g = H[6], h = H[7];

        for (int t = 0; t < 64; ++t) {
            uint32_t T1 = h + sig1(e) + choose(e, f, g) + K[t] + W[t];
            uint32_t T2 = sig0(a) + majority(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        H[0] += a; H[1] += b; H[2] += c; H[3] += d;
        H[4] += e; H[5] += f; H[6] += g; H[7] += h;
    }

    std::ostringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << std::setfill('0') << std::setw(8) << H[i];
    }
    return ss.str();
}

std::string PasswordHasher::generateSalt(size_t byteCount) {
    std::vector<uint8_t> buffer(byteCount);
#if defined(_WIN32)
    NTSTATUS status = BCryptGenRandom(NULL, buffer.data(), static_cast<ULONG>(byteCount), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (!BCRYPT_SUCCESS(status)) {
        std::random_device rd;
        for (size_t i = 0; i < byteCount; ++i) buffer[i] = static_cast<uint8_t>(rd() & 0xFF);
    }
#else
    std::random_device rd;
    for (size_t i = 0; i < byteCount; ++i) buffer[i] = static_cast<uint8_t>(rd() & 0xFF);
#endif

    std::ostringstream ss;
    for (uint8_t b : buffer) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    }
    return ss.str();
}

std::string PasswordHasher::pbkdf2(const std::string& password, const std::string& salt, uint32_t iterations) {
    std::vector<uint8_t> key(32);
#if defined(_WIN32)
    BCRYPT_ALG_HANDLE hAlg = NULL;
    NTSTATUS status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
    if (BCRYPT_SUCCESS(status)) {
        status = BCryptDeriveKeyPBKDF2(hAlg,
                                       reinterpret_cast<PUCHAR>(const_cast<char*>(password.data())),
                                       static_cast<ULONG>(password.length()),
                                       reinterpret_cast<PUCHAR>(const_cast<char*>(salt.data())),
                                       static_cast<ULONG>(salt.length()),
                                       iterations,
                                       key.data(),
                                       static_cast<ULONG>(key.size()),
                                       0);
        BCryptCloseAlgorithmProvider(hAlg, 0);
        if (BCRYPT_SUCCESS(status)) {
            std::ostringstream ss;
            for (uint8_t b : key) {
                ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
            }
            return ss.str();
        }
    }
#endif
    // Fallback software key stretching if CNG is unavailable
    std::string digest = sha256(salt + ":" + password);
    for (uint32_t i = 1; i < iterations; ++i) {
        digest = sha256(digest + ":" + password);
    }
    return digest;
}

std::string PasswordHasher::hashPassword(const std::string& plainPassword) {
    std::string salt = generateSalt(16);
    std::string derivedKey = pbkdf2(plainPassword, salt, PBKDF2_RECOMMENDED_ITERATIONS);
    return "PBKDF2$" + std::to_string(PBKDF2_RECOMMENDED_ITERATIONS) + "$" + salt + "$" + derivedKey;
}

bool PasswordHasher::isLegacyHash(const std::string& storedHash) {
    if (storedHash.rfind("PBKDF2$", 0) != 0) {
        return true; // Legacy SHA-256 or transitional format
    }
    // Enable future work-factor upgrade if stored iterations are less than current recommendation
    size_t firstDollar = storedHash.find('$');
    size_t secondDollar = storedHash.find('$', firstDollar + 1);
    if (secondDollar != std::string::npos) {
        try {
            uint32_t iters = static_cast<uint32_t>(std::stoul(storedHash.substr(firstDollar + 1, secondDollar - (firstDollar + 1))));
            if (iters < PBKDF2_RECOMMENDED_ITERATIONS) {
                return true; // Eligible for automatic work factor upgrade
            }
        } catch (...) {
            return true;
        }
    }
    return false;
}

bool PasswordHasher::constantTimeCompare(const std::string& a, const std::string& b) {
    if (a.length() != b.length()) return false;
    unsigned char result = 0;
    for (size_t i = 0; i < a.length(); ++i) {
        result |= static_cast<unsigned char>(a[i] ^ b[i]);
    }
    return result == 0;
}

bool PasswordHasher::verifyPassword(const std::string& plainPassword, const std::string& storedHash) {
    // 1. NIST SP 800-132 PBKDF2-HMAC-SHA256 Format: PBKDF2$<iterations>$<salt>$<hash>
    if (storedHash.rfind("PBKDF2$", 0) == 0) {
        size_t firstDollar = storedHash.find('$');
        size_t secondDollar = storedHash.find('$', firstDollar + 1);
        size_t thirdDollar = (secondDollar != std::string::npos) ? storedHash.find('$', secondDollar + 1) : std::string::npos;
        if (secondDollar != std::string::npos && thirdDollar != std::string::npos) {
            uint32_t iters = static_cast<uint32_t>(std::stoul(storedHash.substr(firstDollar + 1, secondDollar - (firstDollar + 1))));
            std::string salt = storedHash.substr(secondDollar + 1, thirdDollar - (secondDollar + 1));
            std::string expectedDigest = storedHash.substr(thirdDollar + 1);
            std::string actualDigest = pbkdf2(plainPassword, salt, iters);
            return constantTimeCompare(actualDigest, expectedDigest);
        }
    }

    // 2. Salted Single-Round Hash Format (transitional): SALTED$<salt>$<hash>
    if (storedHash.rfind("SALTED$", 0) == 0) {
        size_t firstDollar = storedHash.find('$');
        size_t secondDollar = storedHash.find('$', firstDollar + 1);
        if (secondDollar != std::string::npos) {
            std::string salt = storedHash.substr(firstDollar + 1, secondDollar - (firstDollar + 1));
            std::string expectedDigest = storedHash.substr(secondDollar + 1);
            std::string actualDigest = sha256(salt + ":" + plainPassword);
            return constantTimeCompare(actualDigest, expectedDigest);
        }
    }

    // 3. Standard SHA-256 Verification (legacy compatibility for initial seeds)
    std::string inputHash = sha256(plainPassword);
    if (constantTimeCompare(inputHash, storedHash)) {
        return true;
    }

    return false;
}
