#include <iostream>
#include <string>
#include <memory>
#include <csignal>

#include "api/httplib.h"
#include "database/Database.h"
#include "database/UserRepository.h"
#include "database/RentalRepository.h"
#include "database/BookingRepository.h"
#include "database/PaymentRepository.h"

#include "services/AuthenticationService.h"
#include "services/RentalService.h"
#include "services/BookingService.h"
#include "services/BillingService.h"
#include "services/PaymentService.h"
#include "services/ReturnService.h"
#include "services/ReportService.h"

#include "api/AuthController.h"
#include "api/RentalController.h"
#include "api/BookingController.h"
#include "api/PaymentController.h"
#include "api/AdminController.h"
#include "api/ReportController.h"

#include "utils/Logger.h"

namespace {
    std::unique_ptr<httplib::Server> g_server = nullptr;

    void signalHandler(int signum) {
        Logger::info("Interrupt signal (" + std::to_string(signum) + ") received. Shutting down server gracefully...");
        if (g_server) {
            g_server->stop();
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "========================================================\n"
              << "   SMART RENTAL MANAGEMENT SYSTEM (C++17 & MySQL 8.0)   \n"
              << "   Advanced Academic Project - Second Year Engineering   \n"
              << "========================================================\n"
              << std::endl;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try {
        // 1. Load Database Configuration
        std::string configPath = "config/database.json";
        if (argc > 1) {
            configPath = argv[1];
        }
        Logger::info("Loading configuration from " + configPath + "...");
        Database::loadConfigFromFile(configPath);

        // 2. Initialize Database Connection
        Database& db = Database::getInstance();
        try {
            db.connect();
            Logger::info("Database connection established successfully!");
        } catch (const std::exception& dbEx) {
            Logger::warn(std::string("Note on MySQL Connection: ") + dbEx.what());
            Logger::warn("Make sure MySQL Server is running and schema.sql / seed.sql have been executed in MySQL Workbench.");
        }

        // 3. Initialize Repositories (Repository Pattern)
        UserRepository userRepo(db);
        RentalRepository rentalRepo(db);
        BookingRepository bookingRepo(db);
        PaymentRepository paymentRepo(db);

        // 4. Initialize Services (Service Layer Pattern)
        AuthenticationService authService(userRepo);
        RentalService rentalService(rentalRepo);
        BookingService bookingService(bookingRepo, rentalRepo);
        BillingService billingService(paymentRepo, bookingRepo);
        PaymentService paymentService(paymentRepo);
        ReturnService returnService(paymentRepo);
        ReportService reportService(paymentRepo, rentalRepo, bookingRepo, userRepo);

        // 5. Initialize HTTP Server (cpp-httplib)
        g_server = std::make_unique<httplib::Server>();

        // 6. Register REST API Controllers
        AuthController authController(authService, userRepo);
        authController.registerRoutes(*g_server);

        RentalController rentalController(rentalService, authService);
        rentalController.registerRoutes(*g_server);

        BookingController bookingController(bookingService, authService);
        bookingController.registerRoutes(*g_server);

        PaymentController paymentController(paymentService, billingService, returnService, authService);
        paymentController.registerRoutes(*g_server);

        AdminController adminController(userRepo, reportService, authService);
        adminController.registerRoutes(*g_server);

        ReportController reportController(reportService, authService);
        reportController.registerRoutes(*g_server);

        // 7. Mount Static Frontend Files
        std::string frontendDir = "./frontend";
        if (!g_server->set_mount_point("/", frontendDir.c_str())) {
            // Try parent path if running from build/
            frontendDir = "../frontend";
            g_server->set_mount_point("/", frontendDir.c_str());
        }
        Logger::info("Mounted static frontend web assets from: " + frontendDir);

        // Trusted Origin CORS pre-flight handler
        g_server->Options(R"(.*)", [](const httplib::Request& req, httplib::Response& res) {
            std::string origin = req.get_header_value("Origin");
            if (!origin.empty() && !ApiControllerBase::isTrustedOrigin(origin)) {
                res.status = 403;
                res.set_content("{\"error\":\"CORS origin denied\"}", "application/json");
                return;
            }
            ApiControllerBase::setCorsHeaders(res, origin);
            res.status = 204;
        });

        // Set CORS headers for all responses if origin is trusted; omit entirely for untrusted origins
        g_server->set_post_routing_handler([](const httplib::Request& req, httplib::Response& res) {
            std::string origin = req.get_header_value("Origin");
            if (!origin.empty() && ApiControllerBase::isTrustedOrigin(origin)) {
                res.set_header("Access-Control-Allow-Origin", origin);
                res.set_header("Vary", "Origin");
                res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
                res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            }
        });

        // Root redirect to index.html
        g_server->Get("/", [](const httplib::Request&, httplib::Response& res) {
            res.set_redirect("/index.html");
        });

        // Health check endpoint
        g_server->Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
            res.set_content("{\"status\":\"healthy\",\"engine\":\"C++17\",\"database\":\"MySQL 8.0\"}", "application/json");
        });

        // 8. Launch Server
        std::string host = "0.0.0.0";
        int port = 8080;
        std::cout << "\n--------------------------------------------------------" << std::endl;
        std::cout << " Server is RUNNING at:  http://localhost:" << port << std::endl;
        std::cout << " Open your web browser to: http://localhost:" << port << std::endl;
        std::cout << " Admin Login: admin@smartrental.com | Password: Password@123" << std::endl;
        std::cout << " Customer Login: aarav.sharma@gmail.com | Password: Password@123" << std::endl;
        std::cout << " Press Ctrl+C to stop the server." << std::endl;
        std::cout << "--------------------------------------------------------\n" << std::endl;

        g_server->listen(host.c_str(), port);

        Logger::info("Server stopped.");
    } catch (const std::exception& ex) {
        Logger::error(std::string("Fatal Server Exception: ") + ex.what());
        return 1;
    }

    return 0;
}
