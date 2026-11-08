#ifndef RENTAL_ITEM_FACTORY_H
#define RENTAL_ITEM_FACTORY_H

#include "RentalItem.h"
#include "Car.h"
#include "Bike.h"
#include "House.h"
#include "Furniture.h"
#include <memory>
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Factory Design Pattern & Smart Pointers (std::unique_ptr)
 * Creates polymorphically typed derived RentalItem objects dynamically
 * based on category discriminator string without coupling callers.
 * ============================================================================
 */
class RentalItemFactory {
public:
    static std::unique_ptr<RentalItem> createItem(
        const std::string& category,
        int id,
        const std::string& title,
        const std::string& description,
        double dailyRate,
        double securityDeposit,
        const std::string& location,
        const std::string& status = "available",
        const std::string& imageUrl = "",
        const std::string& createdAt = ""
    );
};

#endif // RENTAL_ITEM_FACTORY_H
