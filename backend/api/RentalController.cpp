#include "RentalController.h"
#include "../utils/Exceptions.h"
#include <sstream>

RentalController::RentalController(RentalService& rental, AuthenticationService& auth)
    : m_rentalService(rental), m_authService(auth) {}

void RentalController::registerRoutes(httplib::Server& svr) {
    // GET /api/categories
    svr.Get("/api/categories", [this](const httplib::Request&, httplib::Response& res) {
        auto cats = m_rentalService.getSupportedCategories();
        std::ostringstream ss;
        ss << "{\"success\":true,\"categories\":[";
        for (size_t i = 0; i < cats.size(); ++i) {
            ss << "\"" << cats[i] << "\"";
            if (i + 1 < cats.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });

    // GET /api/items
    svr.Get("/api/items", [this](const httplib::Request& req, httplib::Response& res) {
        std::string cat = req.has_param("category") ? req.get_param_value("category") : "";
        std::string q = req.has_param("query") ? req.get_param_value("query") : "";
        std::string loc = req.has_param("location") ? req.get_param_value("location") : "";
        std::string status = req.has_param("status") ? req.get_param_value("status") : "";
        std::string sort = req.has_param("sort_by") ? req.get_param_value("sort_by") : "";

        double minP = 0.0, maxP = 0.0;
        if (req.has_param("min_price")) {
            try { minP = std::stod(req.get_param_value("min_price")); } catch (...) {}
        }
        if (req.has_param("max_price")) {
            try { maxP = std::stod(req.get_param_value("max_price")); } catch (...) {}
        }

        auto items = m_rentalService.getItems(cat, q, loc, minP, maxP, status, sort);

        std::ostringstream ss;
        ss << "{\"success\":true,\"count\":" << items.size() << ",\"items\":[";
        for (size_t i = 0; i < items.size(); ++i) {
            ss << items[i]->toJson();
            if (i + 1 < items.size()) ss << ",";
        }
        ss << "]}";
        sendJson(res, 200, ss.str());
    });

    // GET /api/items/:id
    svr.Get(R"(/api/items/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            int id = std::stoi(req.matches[1]);
            auto item = m_rentalService.getItemById(id);
            sendJson(res, 200, "{\"success\":true,\"item\":" + item->toJson() + "}");
        } catch (const NotFoundException& e) {
            sendError(res, 404, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // POST /api/items (Create New Item - Admin Only)
    svr.Post("/api/items", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Only administrators can add rental items.");
            return;
        }

        try {
            std::string cat = JsonHelper::extractString(req.body, "category");
            std::string title = JsonHelper::extractString(req.body, "title");
            std::string desc = JsonHelper::extractString(req.body, "description");
            double rate = JsonHelper::extractDouble(req.body, "daily_rate");
            double dep = JsonHelper::extractDouble(req.body, "security_deposit");
            std::string loc = JsonHelper::extractString(req.body, "location");
            std::string img = JsonHelper::extractString(req.body, "image_url");

            int newItemId = 0;
            if (cat == "Car") {
                std::string brand = JsonHelper::extractString(req.body, "brand");
                std::string model = JsonHelper::extractString(req.body, "model");
                int year = JsonHelper::extractInt(req.body, "year", 2023);
                std::string fuel = JsonHelper::extractString(req.body, "fuel_type");
                int seats = JsonHelper::extractInt(req.body, "seating_capacity", 5);
                std::string trans = JsonHelper::extractString(req.body, "transmission");
                std::string reg = JsonHelper::extractString(req.body, "registration_number");
                std::string mil = JsonHelper::extractString(req.body, "mileage");

                Car car(0, title, desc, rate, dep, loc, brand, model, year, fuel, seats, trans, reg, mil, "available", img);
                newItemId = m_rentalService.addCar(car);
            } else if (cat == "Bike") {
                std::string brand = JsonHelper::extractString(req.body, "brand");
                std::string model = JsonHelper::extractString(req.body, "model");
                int year = JsonHelper::extractInt(req.body, "year", 2023);
                int cc = JsonHelper::extractInt(req.body, "engine_capacity_cc", 150);
                std::string btype = JsonHelper::extractString(req.body, "bike_type");
                std::string reg = JsonHelper::extractString(req.body, "registration_number");
                bool helm = JsonHelper::extractBool(req.body, "helmet_included", true);

                Bike bike(0, title, desc, rate, dep, loc, brand, model, year, cc, btype, reg, helm, "available", img);
                newItemId = m_rentalService.addBike(bike);
            } else if (cat == "House") {
                std::string ptype = JsonHelper::extractString(req.body, "property_type");
                int bed = JsonHelper::extractInt(req.body, "bedrooms", 1);
                int bath = JsonHelper::extractInt(req.body, "bathrooms", 1);
                std::string furn = JsonHelper::extractString(req.body, "furnishing");
                int sqft = JsonHelper::extractInt(req.body, "area_sqft", 800);
                std::string addr = JsonHelper::extractString(req.body, "address_line");
                bool park = JsonHelper::extractBool(req.body, "parking_available", true);

                House house(0, title, desc, rate, dep, loc, ptype, bed, bath, furn, sqft, addr, park, "available", img);
                newItemId = m_rentalService.addHouse(house);
            } else if (cat == "Furniture") {
                std::string ftype = JsonHelper::extractString(req.body, "furniture_type");
                std::string mat = JsonHelper::extractString(req.body, "material");
                std::string col = JsonHelper::extractString(req.body, "color");
                std::string cond = JsonHelper::extractString(req.body, "item_condition");
                std::string dim = JsonHelper::extractString(req.body, "dimensions");
                bool asmb = JsonHelper::extractBool(req.body, "assembly_required", false);

                Furniture furn(0, title, desc, rate, dep, loc, ftype, mat, col, cond, dim, asmb, "available", img);
                newItemId = m_rentalService.addFurniture(furn);
            } else {
                sendError(res, 400, "Invalid category: " + cat);
                return;
            }

            std::ostringstream ss;
            ss << "{\"success\":true,\"message\":\"Rental item added successfully\",\"item_id\":" << newItemId << "}";
            sendJson(res, 201, ss.str());
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // PUT /api/items/:id (Update Item - Admin Only)
    svr.Put(R"(/api/items/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Only administrators can edit items.");
            return;
        }

        try {
            int id = std::stoi(req.matches[1]);
            std::string title = JsonHelper::extractString(req.body, "title");
            std::string desc = JsonHelper::extractString(req.body, "description");
            double rate = JsonHelper::extractDouble(req.body, "daily_rate");
            double dep = JsonHelper::extractDouble(req.body, "security_deposit");
            std::string loc = JsonHelper::extractString(req.body, "location");
            std::string status = JsonHelper::extractString(req.body, "status");
            std::string img = JsonHelper::extractString(req.body, "image_url");

            m_rentalService.updateItem(id, title, desc, rate, dep, loc, status, img);
            sendSuccess(res, "Item updated successfully.");
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });

    // DELETE /api/items/:id (Delete Item - Admin Only)
    svr.Delete(R"(/api/items/(\d+))", [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = extractToken(req);
        SessionInfo session;
        if (!m_authService.requireRole(token, "admin", session)) {
            sendError(res, 403, "Access Denied: Only administrators can delete items.");
            return;
        }

        try {
            int id = std::stoi(req.matches[1]);
            m_rentalService.deleteItem(id);
            sendSuccess(res, "Item deleted successfully.");
        } catch (const ValidationException& e) {
            sendError(res, 400, e.what());
        } catch (const std::exception& e) {
            sendError(res, 500, e.what());
        }
    });
}
