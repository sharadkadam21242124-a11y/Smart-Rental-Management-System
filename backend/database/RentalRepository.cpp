#include "RentalRepository.h"
#include "../utils/Exceptions.h"
#include "../utils/Logger.h"
#include <sstream>

RentalRepository::RentalRepository(Database& db) : m_db(db) {}

std::shared_ptr<RentalItem> RentalRepository::mapRowToDerivedItem(const QueryRow& r) {
    int id = r.getInt("id");
    std::string cat = r.getString("category");
    std::string title = r.getString("title");
    std::string desc = r.getString("description");
    double rate = r.getDouble("daily_rate");
    double deposit = r.getDouble("security_deposit");
    std::string loc = r.getString("location");
    std::string status = r.getString("status");
    std::string img = r.getString("image_url");
    std::string created = r.getString("created_at");

    if (cat == "Car") {
        std::ostringstream ss;
        ss << "SELECT * FROM cars WHERE item_id = " << id << " LIMIT 1;";
        QueryResult cr = m_db.executeQuery(ss.str());
        if (!cr.empty()) {
            const auto& c = cr[0];
            return std::make_shared<Car>(
                id, title, desc, rate, deposit, loc,
                c.getString("brand"), c.getString("model"), c.getInt("year"),
                c.getString("fuel_type"), c.getInt("seating_capacity"),
                c.getString("transmission"), c.getString("registration_number"),
                c.getString("mileage"), status, img, created
            );
        }
        return std::make_shared<Car>(id, title, desc, rate, deposit, loc, "Tata", "Standard", 2023, "Petrol", 5, "Manual", "N/A", "18 kmpl", status, img, created);
    } else if (cat == "Bike") {
        std::ostringstream ss;
        ss << "SELECT * FROM bikes WHERE item_id = " << id << " LIMIT 1;";
        QueryResult br = m_db.executeQuery(ss.str());
        if (!br.empty()) {
            const auto& b = br[0];
            return std::make_shared<Bike>(
                id, title, desc, rate, deposit, loc,
                b.getString("brand"), b.getString("model"), b.getInt("year"),
                b.getInt("engine_capacity_cc"), b.getString("bike_type"),
                b.getString("registration_number"), b.getBool("helmet_included"),
                status, img, created
            );
        }
        return std::make_shared<Bike>(id, title, desc, rate, deposit, loc, "Honda", "Standard", 2023, 150, "Commuter", "N/A", true, status, img, created);
    } else if (cat == "House") {
        std::ostringstream ss;
        ss << "SELECT * FROM houses WHERE item_id = " << id << " LIMIT 1;";
        QueryResult hr = m_db.executeQuery(ss.str());
        if (!hr.empty()) {
            const auto& h = hr[0];
            return std::make_shared<House>(
                id, title, desc, rate, deposit, loc,
                h.getString("property_type"), h.getInt("bedrooms"), h.getInt("bathrooms"),
                h.getString("furnishing"), h.getInt("area_sqft"),
                h.getString("address_line"), h.getBool("parking_available"),
                status, img, created
            );
        }
        return std::make_shared<House>(id, title, desc, rate, deposit, loc, "Apartment", 2, 2, "Furnished", 1200, loc, true, status, img, created);
    } else if (cat == "Furniture") {
        std::ostringstream ss;
        ss << "SELECT * FROM furniture WHERE item_id = " << id << " LIMIT 1;";
        QueryResult fr = m_db.executeQuery(ss.str());
        if (!fr.empty()) {
            const auto& f = fr[0];
            return std::make_shared<Furniture>(
                id, title, desc, rate, deposit, loc,
                f.getString("furniture_type"), f.getString("material"),
                f.getString("color"), f.getString("item_condition"),
                f.getString("dimensions"), f.getBool("assembly_required"),
                status, img, created
            );
        }
        return std::make_shared<Furniture>(id, title, desc, rate, deposit, loc, "Living Room", "Solid Wood", "Brown", "Like New", "Standard", false, status, img, created);
    }

    return nullptr;
}

std::vector<std::shared_ptr<RentalItem>> RentalRepository::findItems() {
    return findItems("", "", "", 0.0, 0.0, "", "");
}

std::vector<std::shared_ptr<RentalItem>> RentalRepository::findItems(const std::string& category) {
    return findItems(category, "", "", 0.0, 0.0, "", "");
}

std::vector<std::shared_ptr<RentalItem>> RentalRepository::findItems(
    const std::string& category,
    const std::string& query,
    const std::string& location,
    double minPrice,
    double maxPrice,
    const std::string& status,
    const std::string& sortBy
) {
    std::vector<std::shared_ptr<RentalItem>> items;
    std::ostringstream ss;
    ss << "SELECT * FROM rental_items WHERE 1=1";

    if (!category.empty()) {
        ss << " AND category = '" << m_db.escapeString(category) << "'";
    }
    if (!status.empty()) {
        ss << " AND status = '" << m_db.escapeString(status) << "'";
    }
    if (!location.empty()) {
        ss << " AND location LIKE '%" << m_db.escapeString(location) << "%'";
    }
    if (!query.empty()) {
        std::string sq = m_db.escapeString(query);
        ss << " AND (title LIKE '%" << sq << "%' OR description LIKE '%" << sq << "%')";
    }
    if (minPrice > 0.0) {
        ss << " AND daily_rate >= " << minPrice;
    }
    if (maxPrice > 0.0) {
        ss << " AND daily_rate <= " << maxPrice;
    }

    if (sortBy == "price_asc") {
        ss << " ORDER BY daily_rate ASC";
    } else if (sortBy == "price_desc") {
        ss << " ORDER BY daily_rate DESC";
    } else {
        ss << " ORDER BY id ASC";
    }

    QueryResult res = m_db.executeQuery(ss.str());
    for (const auto& row : res.getRows()) {
        auto item = mapRowToDerivedItem(row);
        if (item) items.push_back(item);
    }
    return items;
}

std::shared_ptr<RentalItem> RentalRepository::findById(int id) {
    std::ostringstream ss;
    ss << "SELECT * FROM rental_items WHERE id = " << id << " LIMIT 1;";
    QueryResult res = m_db.executeQuery(ss.str());
    if (res.empty()) return nullptr;
    return mapRowToDerivedItem(res[0]);
}

int RentalRepository::createCar(const Car& car) {
    TransactionGuard guard(m_db);
    std::ostringstream ssBase;
    ssBase << "INSERT INTO rental_items (title, category, description, daily_rate, security_deposit, location, status, image_url) "
           << "VALUES ('" << m_db.escapeString(car.getTitle()) << "', 'Car', '"
           << m_db.escapeString(car.getDescription()) << "', " << car.getDailyRate() << ", "
           << car.getSecurityDeposit() << ", '" << m_db.escapeString(car.getLocation()) << "', '"
           << m_db.escapeString(car.getStatus()) << "', '" << m_db.escapeString(car.getImageUrl()) << "');";
    m_db.executeUpdate(ssBase.str());
    int itemId = static_cast<int>(m_db.getLastInsertId());

    std::ostringstream ssCar;
    ssCar << "INSERT INTO cars (item_id, brand, model, year, fuel_type, seating_capacity, transmission, registration_number, mileage) "
          << "VALUES (" << itemId << ", '" << m_db.escapeString(car.getBrand()) << "', '"
          << m_db.escapeString(car.getModel()) << "', " << car.getYear() << ", '"
          << m_db.escapeString(car.getFuelType()) << "', " << car.getSeatingCapacity() << ", '"
          << m_db.escapeString(car.getTransmission()) << "', '"
          << m_db.escapeString(car.getRegistrationNumber()) << "', '"
          << m_db.escapeString(car.getMileage()) << "');";
    m_db.executeUpdate(ssCar.str());
    guard.commit();
    return itemId;
}

int RentalRepository::createBike(const Bike& bike) {
    TransactionGuard guard(m_db);
    std::ostringstream ssBase;
    ssBase << "INSERT INTO rental_items (title, category, description, daily_rate, security_deposit, location, status, image_url) "
           << "VALUES ('" << m_db.escapeString(bike.getTitle()) << "', 'Bike', '"
           << m_db.escapeString(bike.getDescription()) << "', " << bike.getDailyRate() << ", "
           << bike.getSecurityDeposit() << ", '" << m_db.escapeString(bike.getLocation()) << "', '"
           << m_db.escapeString(bike.getStatus()) << "', '" << m_db.escapeString(bike.getImageUrl()) << "');";
    m_db.executeUpdate(ssBase.str());
    int itemId = static_cast<int>(m_db.getLastInsertId());

    std::ostringstream ssBike;
    ssBike << "INSERT INTO bikes (item_id, brand, model, year, engine_capacity_cc, bike_type, registration_number, helmet_included) "
           << "VALUES (" << itemId << ", '" << m_db.escapeString(bike.getBrand()) << "', '"
           << m_db.escapeString(bike.getModel()) << "', " << bike.getYear() << ", "
           << bike.getEngineCapacityCc() << ", '" << m_db.escapeString(bike.getBikeType()) << "', '"
           << m_db.escapeString(bike.getRegistrationNumber()) << "', "
           << (bike.isHelmetIncluded() ? 1 : 0) << ");";
    m_db.executeUpdate(ssBike.str());
    guard.commit();
    return itemId;
}

int RentalRepository::createHouse(const House& house) {
    TransactionGuard guard(m_db);
    std::ostringstream ssBase;
    ssBase << "INSERT INTO rental_items (title, category, description, daily_rate, security_deposit, location, status, image_url) "
           << "VALUES ('" << m_db.escapeString(house.getTitle()) << "', 'House', '"
           << m_db.escapeString(house.getDescription()) << "', " << house.getDailyRate() << ", "
           << house.getSecurityDeposit() << ", '" << m_db.escapeString(house.getLocation()) << "', '"
           << m_db.escapeString(house.getStatus()) << "', '" << m_db.escapeString(house.getImageUrl()) << "');";
    m_db.executeUpdate(ssBase.str());
    int itemId = static_cast<int>(m_db.getLastInsertId());

    std::ostringstream ssHouse;
    ssHouse << "INSERT INTO houses (item_id, property_type, bedrooms, bathrooms, furnishing, area_sqft, address_line, parking_available) "
            << "VALUES (" << itemId << ", '" << m_db.escapeString(house.getPropertyType()) << "', "
            << house.getBedrooms() << ", " << house.getBathrooms() << ", '"
            << m_db.escapeString(house.getFurnishing()) << "', " << house.getAreaSqft() << ", '"
            << m_db.escapeString(house.getAddressLine()) << "', "
            << (house.isParkingAvailable() ? 1 : 0) << ");";
    m_db.executeUpdate(ssHouse.str());
    guard.commit();
    return itemId;
}

int RentalRepository::createFurniture(const Furniture& furniture) {
    TransactionGuard guard(m_db);
    std::ostringstream ssBase;
    ssBase << "INSERT INTO rental_items (title, category, description, daily_rate, security_deposit, location, status, image_url) "
           << "VALUES ('" << m_db.escapeString(furniture.getTitle()) << "', 'Furniture', '"
           << m_db.escapeString(furniture.getDescription()) << "', " << furniture.getDailyRate() << ", "
           << furniture.getSecurityDeposit() << ", '" << m_db.escapeString(furniture.getLocation()) << "', '"
           << m_db.escapeString(furniture.getStatus()) << "', '" << m_db.escapeString(furniture.getImageUrl()) << "');";
    m_db.executeUpdate(ssBase.str());
    int itemId = static_cast<int>(m_db.getLastInsertId());

    std::ostringstream ssFurn;
    ssFurn << "INSERT INTO furniture (item_id, furniture_type, material, color, item_condition, dimensions, assembly_required) "
           << "VALUES (" << itemId << ", '" << m_db.escapeString(furniture.getFurnitureType()) << "', '"
           << m_db.escapeString(furniture.getMaterial()) << "', '" << m_db.escapeString(furniture.getColor()) << "', '"
           << m_db.escapeString(furniture.getItemCondition()) << "', '"
           << m_db.escapeString(furniture.getDimensions()) << "', "
           << (furniture.isAssemblyRequired() ? 1 : 0) << ");";
    m_db.executeUpdate(ssFurn.str());
    guard.commit();
    return itemId;
}

bool RentalRepository::updateItem(int id, const std::string& title, const std::string& desc,
                                  double rate, double deposit, const std::string& location,
                                  const std::string& status, const std::string& imageUrl) {
    std::ostringstream ss;
    ss << "UPDATE rental_items SET title = '" << m_db.escapeString(title)
       << "', description = '" << m_db.escapeString(desc)
       << "', daily_rate = " << rate
       << ", security_deposit = " << deposit
       << ", location = '" << m_db.escapeString(location)
       << "', status = '" << m_db.escapeString(status)
       << "', image_url = '" << m_db.escapeString(imageUrl)
       << "' WHERE id = " << id << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool RentalRepository::deleteItem(int id) {
    // Check if there are active bookings
    std::ostringstream ssChk;
    ssChk << "SELECT COUNT(*) AS active_count FROM bookings "
          << "WHERE item_id = " << id << " AND booking_status IN ('confirmed', 'active');";
    QueryResult res = m_db.executeQuery(ssChk.str());
    if (!res.empty() && res[0].getInt("active_count") > 0) {
        throw ValidationException("Cannot delete item: active or confirmed bookings currently exist.");
    }

    std::ostringstream ss;
    ss << "DELETE FROM rental_items WHERE id = " << id << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

bool RentalRepository::updateStatus(int id, const std::string& newStatus) {
    std::ostringstream ss;
    ss << "UPDATE rental_items SET status = '" << m_db.escapeString(newStatus)
       << "' WHERE id = " << id << ";";
    return m_db.executeUpdate(ss.str()) > 0;
}

int RentalRepository::getTotalItemCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM rental_items;");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}

int RentalRepository::getAvailableItemCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM rental_items WHERE status = 'available';");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}

int RentalRepository::getRentedItemCount() {
    QueryResult res = m_db.executeQuery("SELECT COUNT(*) AS total FROM rental_items WHERE status = 'rented';");
    if (res.empty()) return 0;
    return res[0].getInt("total");
}

std::map<std::string, int> RentalRepository::getCategoryCounts() {
    std::map<std::string, int> counts;
    QueryResult res = m_db.executeQuery("SELECT category, COUNT(*) AS cnt FROM rental_items GROUP BY category;");
    for (const auto& row : res.getRows()) {
        counts[row.getString("category")] = row.getInt("cnt");
    }
    return counts;
}
