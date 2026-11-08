#include "RentalItemFactory.h"
#include "../utils/Exceptions.h"

std::unique_ptr<RentalItem> RentalItemFactory::createItem(
    const std::string& category,
    int id,
    const std::string& title,
    const std::string& description,
    double dailyRate,
    double securityDeposit,
    const std::string& location,
    const std::string& status,
    const std::string& imageUrl,
    const std::string& createdAt
) {
    if (category == "Car") {
        return std::make_unique<Car>(
            id, title, description, dailyRate, securityDeposit, location,
            "Standard", "Sedan", 2023, "Petrol", 5, "Manual", "UNREGISTERED",
            "18 kmpl", status, imageUrl, createdAt
        );
    } else if (category == "Bike") {
        return std::make_unique<Bike>(
            id, title, description, dailyRate, securityDeposit, location,
            "Standard", "Commuter", 2023, 150, "Commuter", "UNREGISTERED",
            true, status, imageUrl, createdAt
        );
    } else if (category == "House") {
        return std::make_unique<House>(
            id, title, description, dailyRate, securityDeposit, location,
            "Apartment", 2, 2, "Furnished", 1200, location, true,
            status, imageUrl, createdAt
        );
    } else if (category == "Furniture") {
        return std::make_unique<Furniture>(
            id, title, description, dailyRate, securityDeposit, location,
            "Living Room", "Solid Wood", "Natural", "Like New",
            "Standard", false, status, imageUrl, createdAt
        );
    }

    throw ValidationException("Unsupported rental item category: " + category);
}
