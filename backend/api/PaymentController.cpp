#include "PaymentController.h"
#include "../utils/Exceptions.h"
#include <sstream>

PaymentController::PaymentController(PaymentService& pay, BillingService& billing,
                                     ReturnService& ret, AuthenticationService& auth)
    : m_payService(pay), m_billingService(billing),
      m_returnService(ret), m_authService(auth) {}

void PaymentController::registerRoutes(httplib::Server& svr) {
    // POST /api/payments (Record demo transaction)
    svr.Post("/api/payments", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to make payments.");
            return;
        }

        try {
            int bookingId = JsonHelper::extractInt(req.body, "booking_id");
            double amount = JsonHelper::extractDouble(req.body, "amount");
            std::string method = JsonHelper::extractString(req.body, "payment_method");
            std::string type = JsonHelper::extractString(req.body, "payment_type");
            std::string remarks = JsonHelper::extractString(req.body, "remarks");

            if (type.empty()) type = "Rental Payment";
            if (remarks.empty()) remarks = "Demo payment processed successfully";

            // Verify booking ownership for customer role
            auto booking = m_billingService.getBookingById(bookingId);
            if (!booking) {
                sendError(res, 404, "Booking not found with ID: " + std::to_string(bookingId));
                return;
            }

            int custId = session.customerOrAdminId;
            if (session.role != "admin" && booking->getCustomerId() != custId) {
                sendError(res, 403, "Access denied: You can only make payments for your own bookings.");
                return;
            }

            if (session.role == "admin") {
                custId = booking->getCustomerId();
            }

            auto payment = m_payService.processDemoPayment(bookingId, custId, amount, method, type, remarks);

            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Demo payment recorded successfully!\",\"is_demo_gateway\":true,\"payment\":"
               << payment->toJson() << "}";
            sendJson(res, 201, ss.str());
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // GET /api/payments
    svr.Get("/api/payments", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to view transactions.");
            return;
        }

        std::vector<std::shared_ptr<Payment>> payments;
        if (session.role == "admin") {
            payments = m_payService.getAllPayments();
        } else {
            payments = m_payService.getCustomerPayments(session.customerOrAdminId);
        }

        std::ostringstream ss;
        ss << "{\"success\":true,\"count\":" << payments.size() << ",\"payments\":[";
        for (size_t i = 0; i < payments.size(); ++i) {
            ss << payments[i]->toJson();
            if (i + 1 < payments.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });

    // GET /api/invoices/:booking_id (Secure Customer & Admin access)
    svr.Get(R"(/api/invoices/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.validateSession(token, session)) {
            sendError(res, 401, "Please log in to view invoices.");
            return;
        }

        try {
            int bookingId = std::stoi(req.matches[1]);
            auto invoice = m_billingService.getInvoiceByBookingId(bookingId);

            // Authorization: Customers can only view invoices for their own bookings
            if (session.role != "admin") {
                auto booking = m_billingService.getBookingById(bookingId);
                if (booking && booking->getCustomerId() != session.customerOrAdminId) {
                    sendError(res, 403, "Access denied: You are not authorized to view this invoice.");
                    return;
                }
            }

            sendJson(res, 200, "{\"success\":true,\"invoice\":" + invoice->toJson() + "}");
        } catch (const NotFoundException& e) {
            sendError(res, 404, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // POST /api/returns (Check-in item return)
    svr.Post("/api/returns", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Only administrators can process returns.");
            return;
        }

        try {
            int bookingId = JsonHelper::extractInt(req.body, "booking_id");
            std::string actDate = JsonHelper::extractString(req.body, "actual_return_date");
            double damage = JsonHelper::extractDouble(req.body, "damage_charges");
            std::string cond = JsonHelper::extractString(req.body, "item_condition");
            std::string notes = JsonHelper::extractString(req.body, "notes");

            auto ret = m_returnService.processCheckIn(bookingId, actDate, damage, cond, notes, session.customerOrAdminId);
            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Return processed and item checked in successfully!\",\"return\":"
               << ret->toJson() << "}";
            sendJson(res, 201, ss.str());
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // GET /api/returns
    svr.Get("/api/returns", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Only administrators can view return records.");
            return;
        }

        auto returns = m_returnService.getAllReturns();
        std::ostringstream ss;
        ss << "{\"success\":true,\"count\":" << returns.size() << ",\"returns\":[";
        for (size_t i = 0; i < returns.size(); ++i) {
            ss << returns[i]->toJson();
            if (i + 1 < returns.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });
}
