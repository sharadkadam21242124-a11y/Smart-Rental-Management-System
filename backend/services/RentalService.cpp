#include "RentalService.h"
#include "../utils/Exceptions.h"

RentalService::RentalService(RentalRepository& repo) : m_rentalRepo(repo) {}

std::vector<std::shared_ptr<RentalItem>> RentalService::getItems(
    const std::string& category,
    const std::string& query,
    const std::string& location,
    double minPrice,
    double maxPrice,
    const std::string& status,
    const std::string& sortBy
) {
    return m_rentalRepo.findItems(category, query, location, minPrice, maxPrice, status, sortBy);
}

std::shared_ptr<RentalItem> RentalService::getItemById(int id) {
    auto item = m_rentalRepo.findById(id);
    if (!item) throw NotFoundException("Rental item not found with ID: " + std::to_string(id));
    return item;
}

int RentalService::addCar(const Car& car) { return m_rentalRepo.createCar(car); }
int RentalService::addBike(const Bike& bike) { return m_rentalRepo.createBike(bike); }
int RentalService::addHouse(const House& house) { return m_rentalRepo.createHouse(house); }
int RentalService::addFurniture(const Furniture& furniture) { return m_rentalRepo.createFurniture(furniture); }

bool RentalService::updateItem(int id, const std::string& title, const std::string& desc,
                              double rate, double deposit, const std::string& location,
                              const std::string& status, const std::string& imageUrl) {
    return m_rentalRepo.updateItem(id, title, desc, rate, deposit, location, status, imageUrl);
}

bool RentalService::deleteItem(int id) {
    return m_rentalRepo.deleteItem(id);
}

bool RentalService::setItemStatus(int id, const std::string& status) {
    return m_rentalRepo.updateStatus(id, status);
}

std::vector<std::string> RentalService::getSupportedCategories() const {
    return RentalItem::getSupportedCategories();
}
