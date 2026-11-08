#include "ReportController.h"
#include <sstream>

ReportController::ReportController(ReportService& report, AuthenticationService& auth)
    : m_reportService(report), m_authService(auth) {}

void ReportController::registerRoutes(httplib::Server& svr) {
    // GET /api/reports/revenue
    svr.Get("/api/reports/revenue", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Administrator role required.");
            return;
        }

        std::string report = m_reportService.getRevenueReportJson();
        sendJson(res, 200, "{\"success\":true,\"data\":" + report + "}");
    });

    // GET /api/reports/categories
    svr.Get("/api/reports/categories", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Administrator role required.");
            return;
        }

        std::string report = m_reportService.getCategoryReportJson();
        sendJson(res, 200, "{\"success\":true,\"categories\":" + report + "}");
    });

    // GET /api/reports/export/revenue (CSV download)
    svr.Get("/api/reports/export/revenue", [this](const httplib::Request&, httplib::Response& res) {
        std::string csv = m_reportService.exportRevenueReportCsv();
        res.status = 200;
        res.set_content(csv, "text/csv");
        res.set_header("Content-Disposition", "attachment; filename=\"revenue_report.csv\"");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    // GET /api/reports/export/bookings (CSV download)
    svr.Get("/api/reports/export/bookings", [this](const httplib::Request&, httplib::Response& res) {
        std::string csv = m_reportService.exportBookingsReportCsv();
        res.status = 200;
        res.set_content(csv, "text/csv");
        res.set_header("Content-Disposition", "attachment; filename=\"bookings_report.csv\"");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    // GET /api/reports/export/inventory (CSV download)
    svr.Get("/api/reports/export/inventory", [this](const httplib::Request&, httplib::Response& res) {
        std::string csv = m_reportService.exportInventoryReportCsv();
        res.status = 200;
        res.set_content(csv, "text/csv");
        res.set_header("Content-Disposition", "attachment; filename=\"inventory_report.csv\"");
        res.set_header("Access-Control-Allow-Origin", "*");
    });
}
