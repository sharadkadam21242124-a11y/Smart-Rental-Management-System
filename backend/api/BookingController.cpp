#include "BookingController.h"
#include "../utils/Exceptions.h"
#include <sstream>

BookingController::BookingController(BookingService& booking, AuthenticationService& auth)
    : m_bookingService(booking), m_authService(auth) {}

void BookingController::registerRoutes(httplib::Server& svr) {
    // GET /api/availability
    svr.Get("/api/availability", [this](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("item_id") || !req.has_param("start_date") || !req.has_param("end_date")) {
            sendError(res, 400, "Missing required parameters: item_id, start_date, end_date");
            return;
        }

        int itemId = std::stoi(req.get_param_value("item_id"));
        std::string start = req.get_param_value("start_date");
        std::string end = req.get_param_value("end_date");

        bool avail = m_bookingService.checkAvailability(itemId, start, end);
        std::ostringstream ss;
        ss << "{\"success\":true,\"available\":" << (avail ? "true" : "false")
           << ",\"item_id\":" << itemId
           << ",\"start_date\":\"" << start << "\",\"end_date\":\"" << end << "\"}";
        sendJson(res, 200, ss.str());
    });

    // POST /api/bookings (Create booking)
    svr.Post("/api/bookings", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to place a booking.");
            return;
        }

        try {
            int itemId = JsonHelper::extractInt(req.body, "item_id");
            std::string start = JsonHelper::extractString(req.body, "start_date");
            std::string end = JsonHelper::extractString(req.body, "end_date");

            // Customer ID is automatically retrieved from session for security!
            int customerId = session.customerOrAdminId;
            if (session.role == "admin" && JsonHelper::extractInt(req.body, "customer_id") > 0) {
                customerId = JsonHelper::extractInt(req.body, "customer_id");
            }

            auto booking = m_bookingService.createBooking(customerId, itemId, start, end);
            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Booking created successfully!\",\"booking\":"
               << booking->toJson() << "}";
            sendJson(res, 201, ss.str());
        } catch (const BookingConflictException& e) {
            sendError(res, 409, e.what());
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // GET /api/bookings (List bookings for logged in user or admin)
    svr.Get("/api/bookings", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to view bookings.");
            return;
        }

        std::vector<std::shared_ptr<Booking>> bookings;
        if (session.role == "admin") {
            std::string status = req.has_param("status") ? req.get_param_value("status") : "";
            bookings = m_bookingService.getAllBookings(status);
        } else {
            bookings = m_bookingService.getCustomerBookings(session.customerOrAdminId);
        }

        std::ostringstream ss;
        ss << "{\"success\":true,\"count\":" << bookings.size() << ",\"bookings\":[";
        for (size_t i = 0; i < bookings.size(); ++i) {
            ss << bookings[i]->toJson();
            if (i + 1 < bookings.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });

    // GET /api/bookings/:id
    svr.Get(R"(/api/bookings/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to view booking details.");
            return;
        }

        try {
            int id = std::stoi(req.matches[1]);
            auto booking = m_bookingService.getBookingById(id);

            // Authorization: customer can only view own booking unless admin
            if (session.role != "admin" && booking->getCustomerId() != session.customerOrAdminId) {
                sendError(res, 403, "Access denied to this booking.");
                return;
            }

            sendJson(res, 200, "{\"success\":true,\"booking\":" + booking->toJson() + "}");
        } catch (const NotFoundException& e) {
            sendError(res, 404, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // PUT /api/bookings/:id/cancel
    svr.Put(R"(/api/bookings/(\d+)/cancel)", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to cancel bookings.");
            return;
        }

        try {
            int id = std::stoi(req.matches[1]);
            int custId = (session.role == "admin") ? 0 : session.customerOrAdminId;
            m_bookingService.cancelBooking(id, custId);
            sendSuccess(res, "Booking cancelled successfully.");
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });
}
