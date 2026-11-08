#ifndef FURNITURE_H
#define FURNITURE_H

#include "RentalItem.h"
#include <string>

/**
 * ============================================================================
 * OOP CONCEPT: Hierarchical Inheritance & Polymorphism
 * Furniture derives from RentalItem, specialized for home/office furnishings.
 * ============================================================================
 */
class Furniture : public RentalItem {
private:
    std::string m_furnitureType; // Living Room, Bedroom, Office, Dining, Storage
    std::string m_material;
    std::string m_color;
    std::string m_itemCondition; // Brand New, Like New, Good
    std::string m_dimensions;
    bool m_assemblyRequired;

public:
    Furniture();
    Furniture(int id, const std::string& title, const std::string& description,
              double dailyRate, double securityDeposit, const std::string& location,
              const std::string& furnitureType, const std::string& material,
              const std::string& color, const std::string& condition = "Like New",
              const std::string& dimensions = "", bool assemblyRequired = false,
              const std::string& status = "available",
              const std::string& imageUrl = "",
              const std::string& createdAt = "");
    virtual ~Furniture() override;

    // Getters
    std::string getFurnitureType() const;
    std::string getMaterial() const;
    std::string getColor() const;
    std::string getItemCondition() const;
    std::string getDimensions() const;
    bool isAssemblyRequired() const;

    // Setters
    void setFurnitureType(const std::string& type);
    void setMaterial(const std::string& material);
    void setColor(const std::string& color);
    void setItemCondition(const std::string& condition);
    void setDimensions(const std::string& dim);
    void setAssemblyRequired(bool req);

    // Polymorphic implementations
    virtual std::string getCategory() const override;
    virtual double calculateRent(int days) const override;
    virtual std::string toJson() const override;
};

#endif // FURNITURE_H
