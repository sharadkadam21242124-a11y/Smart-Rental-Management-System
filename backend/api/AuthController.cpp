#include "AuthController.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"

AuthController::AuthController(AuthenticationService& auth, UserRepository& userRepo)
    : m_authService(auth), m_userRepo(userRepo) {}

void AuthController::registerRoutes(httplib::Server& svr) {
    // POST /api/register and /api/auth/register
    auto registerHandler = [this](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string email = JsonHelper::extractString(req.body, "email");
            std::string pass = JsonHelper::extractString(req.body, "password");
            std::string name = JsonHelper::extractString(req.body, "full_name");
            if (name.empty()) name = JsonHelper::extractString(req.body, "name");
            std::string phone = JsonHelper::extractString(req.body, "phone");
            std::string addr = JsonHelper::extractString(req.body, "address");
            std::string idProof = JsonHelper::extractString(req.body, "id_proof_number");
            std::string dl = JsonHelper::extractString(req.body, "driving_license_number");

            std::string token = m_authService.registerCustomer(email, pass, name, phone, addr, idProof, dl);
            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Registration successful! Welcome to Smart Rental.\",\"token\":\""
               << token << "\"}";
            sendJson(res, 201, ss.str());
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    };
    svr.Post("/api/register", registerHandler);
    svr.Post("/api/auth/register", registerHandler);

    // POST /api/login and /api/auth/login
    auto loginHandler = [this](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string email = JsonHelper::extractString(req.body, "email");
            std::string pass = JsonHelper::extractString(req.body, "password");

            auto [token, session] = m_authService.login(email, pass);
            std::ostringstream ss;
            ss << "{\"success\":true,\"token\":\"" << token
               << "\",\"user\":{"
               << JsonHelper::kv("user_id", session.userId)
               << JsonHelper::kvString("email", session.email)
               << JsonHelper::kvString("role", session.role)
               << JsonHelper::kvString("full_name", session.fullName)
               << JsonHelper::kv("account_id", session.customerOrAdminId, false)
               << "}}";
            sendJson(res, 200, ss.str());
        } catch (const AuthenticationException& e) {
            sendError(res, 401, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    };
    svr.Post("/api/login", loginHandler);
    svr.Post("/api/auth/login", loginHandler);

    // POST /api/logout and /api/auth/logout
    auto logoutHandler = [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        if (!token.empty()) {
            m_authService.logout(token);
        }
        sendSuccess(res, "Successfully logged out.");
    };
    svr.Post("/api/logout", logoutHandler);
    svr.Post("/api/auth/logout", logoutHandler);

    // GET /api/profile and /api/auth/profile
    auto getProfileHandler = [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Unauthorized: Please log in to view profile.");
            return;
        }

        if (session.role == "customer") {
            auto cust = m_userRepo.findCustomerByUserId(session.userId);
            if (cust) {
                sendJson(res, 200, "{\"success\":true,\"profile\":" + cust->toJson() + ",\"data\":" + cust->toJson() + "}");
                return;
            }
        } else {
            auto adm = m_userRepo.findAdminByUserId(session.userId);
            if (adm) {
                sendJson(res, 200, "{\"success\":true,\"profile\":" + adm->toJson() + ",\"data\":" + adm->toJson() + "}");
                return;
            }
        }
        sendError(res, 404, "Profile details not found.");
    };
    svr.Get("/api/profile", getProfileHandler);
    svr.Get("/api/auth/profile", getProfileHandler);

    // PUT /api/profile and /api/auth/profile (Update Profile)
    auto putProfileHandler = [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Unauthorized: Please log in to update profile.");
            return;
        }

        try {
            std::string name = JsonHelper::extractString(req.body, "full_name");
            if (name.empty()) name = JsonHelper::extractString(req.body, "name");
            std::string phone = JsonHelper::extractString(req.body, "phone");
            std::string address = JsonHelper::extractString(req.body, "address");
            std::string dept = JsonHelper::extractString(req.body, "department");

            bool ok = m_authService.updateProfile(session.userId, session.role, session.customerOrAdminId, name, phone, address, dept);
            if (ok) {
                sendSuccess(res, "Profile updated successfully.");
            } else {
                sendError(res, 400, "Could not update profile.");
            }
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    };
    svr.Put("/api/profile", putProfileHandler);
    svr.Put("/api/auth/profile", putProfileHandler);

    // POST /api/change-password and /api/auth/change-password
    auto changePasswordHandler = [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Unauthorized: Please log in to change password.");
            return;
        }

        try {
            std::string curr = JsonHelper::extractString(req.body, "currentPassword");
            if (curr.empty()) curr = JsonHelper::extractString(req.body, "current_password");
            std::string newP = JsonHelper::extractString(req.body, "newPassword");
            if (newP.empty()) newP = JsonHelper::extractString(req.body, "new_password");

            bool ok = m_authService.changePassword(session.userId, curr, newP);
            if (ok) {
                sendSuccess(res, "Password changed successfully.");
            } else {
                sendError(res, 400, "Password update failed.");
            }
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const AuthenticationException& e) {
            sendError(res, 401, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    };
    svr.Post("/api/change-password", changePasswordHandler);
    svr.Post("/api/auth/change-password", changePasswordHandler);
}
