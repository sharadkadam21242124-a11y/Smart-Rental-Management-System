#ifndef RENTAL_REPOSITORY_H
#define RENTAL_REPOSITORY_H

#include "../models/RentalItem.h"
#include "../models/Car.h"
#include "../models/Bike.h"
#include "../models/House.h"
#include "../models/Furniture.h"
#include "Database.h"
#include <memory>
#include <vector>
#include <string>
#include <map>

/**
 * ============================================================================
 * OOP CONCEPT: Polymorphism, Factory, Function Overloading, Repository Pattern
 * RentalRepository coordinates the retrieval and persistence of polymorphic
 * RentalItem objects across rental_items, cars, bikes, houses, and furniture.
 * ============================================================================
 */
class RentalRepository {
private:
    Database& m_db;

    // Helper to map a database row into the appropriate derived RentalItem subclass
    std::shared_ptr<RentalItem> mapRowToDerivedItem(const QueryRow& baseRow);

public:
    explicit RentalRepository(Database& db = Database::getInstance());

    // ------------------------------------------------------------------------
    // FUNCTION OVERLOADING: Overloaded find methods for flexible querying
    // ------------------------------------------------------------------------
    std::vector<std::shared_ptr<RentalItem>> findItems();
    std::vector<std::shared_ptr<RentalItem>> findItems(const std::string& category);
    std::vector<std::shared_ptr<RentalItem>> findItems(
        const std::string& category,
        const std::string& query,
        const std::string& location,
        double minPrice,
        double maxPrice,
        const std::string& status = "",
        const std::string& sortBy = ""
    );

    std::shared_ptr<RentalItem> findById(int id);

    // Specialized Polymorphic Creations
    int createCar(const Car& car);
    int createBike(const Bike& bike);
    int createHouse(const House& house);
    int createFurniture(const Furniture& furniture);

    // Update and Deletion
    bool updateItem(int id, const std::string& title, const std::string& desc,
                    double rate, double deposit, const std::string& location,
                    const std::string& status, const std::string& imageUrl);
    bool deleteItem(int id);
    bool updateStatus(int id, const std::string& newStatus);

    // Fleet Statistics
    int getTotalItemCount();
    int getAvailableItemCount();
    int getRentedItemCount();
    std::map<std::string, int> getCategoryCounts();
};

#endif // RENTAL_REPOSITORY_H
