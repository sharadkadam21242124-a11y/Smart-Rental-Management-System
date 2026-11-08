#ifndef TEMPLATE_REPOSITORY_H
#define TEMPLATE_REPOSITORY_H

#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <algorithm>
#include <optional>
#include "../utils/Exceptions.h"

/**
 * ============================================================================
 * OOP CONCEPT: Class Templates & Generic Programming (Requirement #21)
 * TemplateRepository provides a reusable, generic, thread-safe repository
 * container for in-memory caching and generic CRUD operations across any model.
 * ============================================================================
 */
template <typename T>
class TemplateRepository {
protected:
    std::map<int, std::shared_ptr<T>> m_items;
    mutable std::mutex m_mutex;

public:
    TemplateRepository() = default;
    virtual ~TemplateRepository() = default;

    // Add or replace an item
    virtual void save(int id, std::shared_ptr<T> item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_items[id] = item;
    }

    // Retrieve by ID
    virtual std::shared_ptr<T> findById(int id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_items.find(id);
        if (it != m_items.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Retrieve all items
    virtual std::vector<std::shared_ptr<T>> findAll() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::shared_ptr<T>> result;
        result.reserve(m_items.size());
        for (const auto& pair : m_items) {
            result.push_back(pair.second);
        }
        return result;
    }

    // Delete item by ID
    virtual bool deleteById(int id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_items.erase(id) > 0;
    }

    // Total count
    virtual size_t count() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_items.size();
    }

    // Clear repository
    virtual void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_items.clear();
    }

    // Generic search with predicate (Function Template inside Class Template)
    template <typename Predicate>
    std::vector<std::shared_ptr<T>> filter(Predicate pred) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::shared_ptr<T>> matches;
        for (const auto& pair : m_items) {
            if (pred(*(pair.second))) {
                matches.push_back(pair.second);
            }
        }
        return matches;
    }
};

#endif // TEMPLATE_REPOSITORY_H
