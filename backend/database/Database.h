#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>

#if defined(USE_MYSQL_CLIENT)
#include <mysql.h>
#endif

/**
 * ============================================================================
 * OOP CONCEPT: RAII, Singleton Pattern, Exception Handling, Database Abstraction
 * Database encapsulates connection management, query execution, transactions,
 * and prepared statements for MySQL Server.
 * ============================================================================
 */

struct DatabaseConfig {
    std::string host = "127.0.0.1";
    int port = 3306;
    std::string database = "smart_rental_db";
    std::string username = "rental_user";
    std::string password = "RentalUser@2026";
    int poolSize = 5;
    int timeoutSeconds = 10;
};

class QueryRow {
private:
    std::map<std::string, std::string> m_data;

public:
    QueryRow() = default;
    void set(const std::string& col, const std::string& val);
    std::string getString(const std::string& col, const std::string& defaultVal = "") const;
    int getInt(const std::string& col, int defaultVal = 0) const;
    double getDouble(const std::string& col, double defaultVal = 0.0) const;
    bool getBool(const std::string& col, bool defaultVal = false) const;
    bool has(const std::string& col) const;
};

class QueryResult {
private:
    std::vector<QueryRow> m_rows;
    int m_affectedRows = 0;
    long long m_lastInsertId = 0;

public:
    QueryResult() = default;
    void addRow(const QueryRow& row);
    void setAffectedRows(int count);
    void setLastInsertId(long long id);

    const std::vector<QueryRow>& getRows() const;
    size_t size() const;
    bool empty() const;
    const QueryRow& operator[](size_t index) const;
    int getAffectedRows() const;
    long long getLastInsertId() const;
};

class Database {
private:
    static std::unique_ptr<Database> s_instance;
    static std::mutex s_mutex;

    DatabaseConfig m_config;
    bool m_connected;
    std::mutex m_dbMutex;

#if defined(USE_MYSQL_CLIENT)
    MYSQL* m_mysqlHandle;
#else
    void* m_mysqlHandle;
#endif

    // Private Constructor for Singleton Pattern
    explicit Database(const DatabaseConfig& config);

public:
    ~Database();

    // Singleton access
    static Database& getInstance();
    static void initialize(const DatabaseConfig& config);
    static void loadConfigFromFile(const std::string& configFilePath);

    bool connect();
    void disconnect();
    bool isConnected() const;

    // Direct Query & Execution
    QueryResult executeQuery(const std::string& sql);
    int executeUpdate(const std::string& sql);
    long long getLastInsertId();

    // Parameterized SQL Escaping
    std::string escapeString(const std::string& str);

    // Transaction Management
    void beginTransaction();
    void commit();
    void rollback();

    // Health check
    bool testConnection();
};

/**
 * ============================================================================
 * OOP CONCEPT: RAII (Resource Acquisition Is Initialization)
 * TransactionGuard guarantees automatic ROLLBACK if a transaction fails
 * before an explicit commit() is invoked.
 * ============================================================================
 */
class TransactionGuard {
private:
    Database& m_db;
    bool m_committed;

public:
    explicit TransactionGuard(Database& db) : m_db(db), m_committed(false) {
        m_db.beginTransaction();
    }

    ~TransactionGuard() {
        if (!m_committed) {
            try {
                m_db.rollback();
            } catch (...) {
                // Suppress exception in destructor per RAII best practices
            }
        }
    }

    void commit() {
        m_db.commit();
        m_committed = true;
    }
};

#endif // DATABASE_H
