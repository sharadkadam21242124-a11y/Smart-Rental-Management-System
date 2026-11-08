#include "AdminController.h"
#include "../utils/Exceptions.h"
#include <sstream>

AdminController::AdminController(UserRepository& userRepo, ReportService& report, AuthenticationService& auth)
    : m_userRepo(userRepo), m_reportService(report), m_authService(auth) {}

void AdminController::registerRoutes(httplib::Server& svr) {
    // GET /api/admin/dashboard
    svr.Get("/api/admin/dashboard", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Administrator role required.");
            return;
        }

        std::string stats = m_reportService.getDashboardStatsJson();
        sendJson(res, 200, "{\"success\":true,\"stats\":" + stats + "}");
    });

    // GET /api/admin/customers
    svr.Get("/api/admin/customers", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Administrator role required.");
            return;
        }

        auto customers = m_userRepo.getAllCustomers();
        std::ostringstream ss;
        ss << "{\"success\":true,\"count\":" << customers.size() << ",\"customers\":[";
        for (size_t i = 0; i < customers.size(); ++i) {
            ss << customers[i]->toJson();
            if (i + 1 < customers.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });

    // PUT /api/admin/customers/:user_id/status (Enable or Disable customer account)
    svr.Put(R"(/api/admin/customers/(\d+)/status)", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Administrator role required.");
            return;
        }

        try {
            int userId = std::stoi(req.matches[1]);
            bool active = JsonHelper::extractBool(req.body, "is_active", true);
            m_userRepo.setCustomerActiveStatus(userId, active);
            sendSuccess(res, "Customer status updated successfully.");
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });
}
