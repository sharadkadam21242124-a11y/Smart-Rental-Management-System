#ifndef API_CONTROLLER_BASE_H
#define API_CONTROLLER_BASE_H

#include "httplib.h"
#include "../utils/JsonHelper.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Base Controller & Reusable Response Abstractions
 * ApiControllerBase defines standard response helpers, CORS configuration,
 * and authentication token extraction for all REST API endpoints.
 * ============================================================================
 */
class ApiControllerBase {
public:
    static bool isTrustedOrigin(const std::string& origin) {
        if (origin.empty()) return false;
        return (origin == "http://localhost:8080" || origin == "http://127.0.0.1:8080" ||
                origin == "http://localhost:3000" || origin == "http://localhost:5500" ||
                origin == "http://127.0.0.1:5500");
    }

    static void setCorsHeaders(httplib::Response& res, const std::string& origin = "") {
        if (!origin.empty() && isTrustedOrigin(origin)) {
            res.set_header("Access-Control-Allow-Origin", origin);
            res.set_header("Vary", "Origin");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        }
        // Untrusted or empty origins receive no Access-Control-Allow-Origin header
    }

    static void sendJson(httplib::Response& res, int statusCode, const std::string& jsonBody, const std::string& origin = "") {
        res.status = statusCode;
        res.set_content(jsonBody, "application/json");
        setCorsHeaders(res, origin);
    }

    static void sendSuccess(httplib::Response& res, const std::string& message = "Operation successful") {
        sendJson(res, 200, "{\"success\":true,\"message\":\"" + JsonHelper::escapeString(message) + "\"}");
    }

    static void sendError(httplib::Response& res, int statusCode, const std::string& errorMessage) {
        sendJson(res, statusCode, "{\"success\":false,\"error\":\"" + JsonHelper::escapeString(errorMessage) + "\"}");
    }

    static std::string extractToken(const httplib::Request& req) {
        if (req.has_header("Authorization")) {
            std::string auth = req.get_header_value("Authorization");
            if (auth.substr(0, 7) == "Bearer ") {
                return auth.substr(7);
            }
            return auth;
        }
        return "";
    }
};

#endif // API_CONTROLLER_BASE_H
