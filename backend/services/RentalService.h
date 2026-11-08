#ifndef RENTAL_SERVICE_H
#define RENTAL_SERVICE_H

#include "../database/RentalRepository.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

/**
 * ============================================================================
 * OOP CONCEPT: Service Layer Pattern & Function Templates
 * RentalService coordinates catalog filtering, sorting, price evaluations,
 * and category-specific management rules.
 * ============================================================================
 */
class RentalService {
private:
    RentalRepository& m_rentalRepo;

public:
    explicit RentalService(RentalRepository& repo);

    std::vector<std::shared_ptr<RentalItem>> getItems(
        const std::string& category = "",
        const std::string& query = "",
        const std::string& location = "",
        double minPrice = 0.0,
        double maxPrice = 0.0,
        const std::string& status = "",
        const std::string& sortBy = ""
    );

    std::shared_ptr<RentalItem> getItemById(int id);

    // Template Sorting Algorithm (OOP Requirement #20)
    template <typename Comparator>
    static void sortItems(std::vector<std::shared_ptr<RentalItem>>& items, Comparator comp) {
        std::sort(items.begin(), items.end(), comp);
    }

    // Admin Operations
    int addCar(const Car& car);
    int addBike(const Bike& bike);
    int addHouse(const House& house);
    int addFurniture(const Furniture& furniture);

    bool updateItem(int id, const std::string& title, const std::string& desc,
                    double rate, double deposit, const std::string& location,
                    const std::string& status, const std::string& imageUrl);
    bool deleteItem(int id);
    bool setItemStatus(int id, const std::string& status);

    std::vector<std::string> getSupportedCategories() const;
};

#endif // RENTAL_SERVICE_H
