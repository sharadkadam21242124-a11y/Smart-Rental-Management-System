#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>

/**
 * ============================================================================
 * OOP CONCEPT: Utility Class & Function Templates
 * Clean, lightweight JSON serializer and parser for REST API responses
 * and requests without requiring heavy external dependencies.
 * ============================================================================
 */
class JsonHelper {
public:
    static std::string escapeString(const std::string& input) {
        std::ostringstream ss;
        for (char c : input) {
            switch (c) {
                case '"': ss << "\\\""; break;
                case '\\': ss << "\\\\"; break;
                case '\b': ss << "\\b"; break;
                case '\f': ss << "\\f"; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        // skip or escape control chars
                    } else {
                        ss << c;
                    }
                    break;
            }
        }
        return ss.str();
    }

    // Function Template to format key-value pairs
    template <typename T>
    static std::string kv(const std::string& key, const T& value, bool comma = true) {
        std::ostringstream ss;
        ss << "\"" << escapeString(key) << "\":" << value;
        if (comma) ss << ",";
        return ss.str();
    }

    static std::string kvString(const std::string& key, const std::string& value, bool comma = true) {
        std::ostringstream ss;
        ss << "\"" << escapeString(key) << "\":\"" << escapeString(value) << "\"";
        if (comma) ss << ",";
        return ss.str();
    }

    static std::string kvBool(const std::string& key, bool value, bool comma = true) {
        std::ostringstream ss;
        ss << "\"" << escapeString(key) << "\":" << (value ? "true" : "false");
        if (comma) ss << ",";
        return ss.str();
    }

    // Helper to extract a string value by key from a JSON object string
    static std::string extractString(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return "";

        size_t colon = json.find(':', pos + searchKey.length());
        if (colon == std::string::npos) return "";

        size_t quoteStart = json.find('"', colon + 1);
        if (quoteStart == std::string::npos) return "";

        size_t quoteEnd = json.find('"', quoteStart + 1);
        while (quoteEnd != std::string::npos && json[quoteEnd - 1] == '\\') {
            quoteEnd = json.find('"', quoteEnd + 1);
        }
        if (quoteEnd == std::string::npos) return "";

        return json.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
    }

    // Helper to extract numeric/boolean values
    static double extractDouble(const std::string& json, const std::string& key, double defaultVal = 0.0) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return defaultVal;

        size_t colon = json.find(':', pos + searchKey.length());
        if (colon == std::string::npos) return defaultVal;

        size_t start = json.find_first_not_of(" \t\n\r", colon + 1);
        if (start == std::string::npos) return defaultVal;

        // If enclosed in quotes, read string then convert
        if (json[start] == '"') {
            std::string s = extractString(json, key);
            try { return std::stod(s); } catch (...) { return defaultVal; }
        }

        size_t end = json.find_first_of(",}\n\r\t ", start);
        std::string token = json.substr(start, (end == std::string::npos ? json.length() : end) - start);
        try {
            return std::stod(token);
        } catch (...) {
            return defaultVal;
        }
    }

    static int extractInt(const std::string& json, const std::string& key, int defaultVal = 0) {
        return static_cast<int>(extractDouble(json, key, defaultVal));
    }

    static bool extractBool(const std::string& json, const std::string& key, bool defaultVal = false) {
        std::string searchKey = "\"" + key + "\"";
        size_t pos = json.find(searchKey);
        if (pos == std::string::npos) return defaultVal;

        size_t colon = json.find(':', pos + searchKey.length());
        if (colon == std::string::npos) return defaultVal;

        size_t start = json.find_first_not_of(" \t\n\r", colon + 1);
        if (start == std::string::npos) return defaultVal;

        if (json.substr(start, 4) == "true" || json.substr(start, 3) == "\"1\"" || json[start] == '1') return true;
        if (json.substr(start, 5) == "false" || json.substr(start, 3) == "\"0\"" || json[start] == '0') return false;
        return defaultVal;
    }
};

#endif // JSON_HELPER_H
