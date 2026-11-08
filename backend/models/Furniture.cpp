#include "Furniture.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"
#include <sstream>

Furniture::Furniture()
    : RentalItem(), m_furnitureType("Living Room"), m_material("Wood"),
      m_color("Brown"), m_itemCondition("Like New"),
      m_dimensions(""), m_assemblyRequired(false) {}

Furniture::Furniture(int id, const std::string& title, const std::string& description,
                     double dailyRate, double securityDeposit, const std::string& location,
                     const std::string& furnitureType, const std::string& material,
                     const std::string& color, const std::string& condition,
                     const std::string& dimensions, bool assemblyRequired,
                     const std::string& status, const std::string& imageUrl,
                     const std::string& createdAt)
    : RentalItem(id, title, description, dailyRate, securityDeposit, location, status, imageUrl, createdAt),
      m_furnitureType(furnitureType), m_material(material), m_color(color),
      m_itemCondition(condition), m_dimensions(dimensions),
      m_assemblyRequired(assemblyRequired) {
    if (material.empty()) {
        throw ValidationException("Furniture material cannot be empty.");
    }
}

Furniture::~Furniture() {}

std::string Furniture::getFurnitureType() const { return m_furnitureType; }
std::string Furniture::getMaterial() const { return m_material; }
std::string Furniture::getColor() const { return m_color; }
std::string Furniture::getItemCondition() const { return m_itemCondition; }
std::string Furniture::getDimensions() const { return m_dimensions; }
bool Furniture::isAssemblyRequired() const { return m_assemblyRequired; }

void Furniture::setFurnitureType(const std::string& type) { m_furnitureType = type; }
void Furniture::setMaterial(const std::string& material) { m_material = material; }
void Furniture::setColor(const std::string& color) { m_color = color; }
void Furniture::setItemCondition(const std::string& condition) { m_itemCondition = condition; }
void Furniture::setDimensions(const std::string& dim) { m_dimensions = dim; }
void Furniture::setAssemblyRequired(bool req) { m_assemblyRequired = req; }

std::string Furniture::getCategory() const { return "Furniture"; }

double Furniture::calculateRent(int days) const {
    if (days <= 0) return 0.0;
    double base = days * m_dailyRate;
    if (days >= 30) {
        return base * 0.75; // 25% discount for 30+ days
    } else if (days >= 14) {
        return base * 0.85; // 15% discount for 14+ days
    } else if (days >= 7) {
        return base * 0.95; // 5% discount for 7+ days
    }
    return base;
}

std::string Furniture::toJson() const {
    std::ostringstream ss;
    ss << "{"
       << JsonHelper::kv("id", m_id)
       << JsonHelper::kvString("title", m_title)
       << JsonHelper::kvString("category", "Furniture")
       << JsonHelper::kvString("description", m_description)
       << JsonHelper::kv("daily_rate", m_dailyRate)
       << JsonHelper::kv("security_deposit", m_securityDeposit)
       << JsonHelper::kvString("location", m_location)
       << JsonHelper::kvString("status", m_status)
       << JsonHelper::kvString("image_url", m_imageUrl)
       << JsonHelper::kvString("furniture_type", m_furnitureType)
       << JsonHelper::kvString("material", m_material)
       << JsonHelper::kvString("color", m_color)
       << JsonHelper::kvString("item_condition", m_itemCondition)
       << JsonHelper::kvString("dimensions", m_dimensions)
       << JsonHelper::kvBool("assembly_required", m_assemblyRequired)
       << JsonHelper::kvString("created_at", m_createdAt, false)
       << "}";
    return ss.str();
}
