#include "Database.h"
#include "../utils/Logger.h"
#include "../utils/Exceptions.h"
#include "../utils/JsonHelper.h"

#include <fstream>
#include <iostream>

#if defined(USE_MYSQL_CLIENT)
#include <mysql.h>
#endif

// ----------------------------------------------------------------------------
// QueryRow Implementation
// ----------------------------------------------------------------------------
void QueryRow::set(const std::string& col, const std::string& val) {
    m_data[col] = val;
}

std::string QueryRow::getString(const std::string& col, const std::string& defaultVal) const {
    auto it = m_data.find(col);
    if (it != m_data.end()) return it->second;
    return defaultVal;
}

int QueryRow::getInt(const std::string& col, int defaultVal) const {
    auto it = m_data.find(col);
    if (it != m_data.end()) {
        try { return std::stoi(it->second); } catch (...) { return defaultVal; }
    }
    return defaultVal;
}

double QueryRow::getDouble(const std::string& col, double defaultVal) const {
    auto it = m_data.find(col);
    if (it != m_data.end()) {
        try { return std::stod(it->second); } catch (...) { return defaultVal; }
    }
    return defaultVal;
}

bool QueryRow::getBool(const std::string& col, bool defaultVal) const {
    auto it = m_data.find(col);
    if (it != m_data.end()) {
        return (it->second == "1" || it->second == "true" || it->second == "TRUE");
    }
    return defaultVal;
}

bool QueryRow::has(const std::string& col) const {
    return m_data.find(col) != m_data.end();
}

// ----------------------------------------------------------------------------
// QueryResult Implementation
// ----------------------------------------------------------------------------
void QueryResult::addRow(const QueryRow& row) { m_rows.push_back(row); }
void QueryResult::setAffectedRows(int count) { m_affectedRows = count; }
void QueryResult::setLastInsertId(long long id) { m_lastInsertId = id; }

const std::vector<QueryRow>& QueryResult::getRows() const { return m_rows; }
size_t QueryResult::size() const { return m_rows.size(); }
bool QueryResult::empty() const { return m_rows.empty(); }
const QueryRow& QueryResult::operator[](size_t index) const { return m_rows.at(index); }
int QueryResult::getAffectedRows() const { return m_affectedRows; }
long long QueryResult::getLastInsertId() const { return m_lastInsertId; }

// ----------------------------------------------------------------------------
// Database Singleton Implementation
// ----------------------------------------------------------------------------
std::unique_ptr<Database> Database::s_instance = nullptr;
std::mutex Database::s_mutex;

Database::Database(const DatabaseConfig& config)
    : m_config(config), m_connected(false), m_mysqlHandle(nullptr) {}

Database::~Database() {
    disconnect();
}

Database& Database::getInstance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (!s_instance) {
        DatabaseConfig defaultConfig;
        s_instance = std::unique_ptr<Database>(new Database(defaultConfig));
    }
    return *s_instance;
}

void Database::initialize(const DatabaseConfig& config) {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_instance = std::unique_ptr<Database>(new Database(config));
}

void Database::loadConfigFromFile(const std::string& configFilePath) {
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        Logger::warn("Config file not found at " + configFilePath + ", using default configuration.");
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    DatabaseConfig cfg;
    std::string host = JsonHelper::extractString(content, "host");
    if (!host.empty()) cfg.host = host;

    int port = JsonHelper::extractInt(content, "port", 3306);
    if (port > 0) cfg.port = port;

    std::string db = JsonHelper::extractString(content, "database");
    if (!db.empty()) cfg.database = db;

    std::string user = JsonHelper::extractString(content, "username");
    if (!user.empty()) cfg.username = user;

    std::string pass = JsonHelper::extractString(content, "password");
    if (!pass.empty()) cfg.password = pass;

    const char* envPass = std::getenv("DB_PASSWORD");
    if (!envPass) envPass = std::getenv("MYSQL_PASSWORD");
    if (cfg.password.empty() && envPass) {
        cfg.password = envPass;
    }

    initialize(cfg);
    Logger::info("Database configuration loaded from " + configFilePath + " for database: " + cfg.database);
}

bool Database::connect() {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
    if (m_connected) return true;

#if defined(USE_MYSQL_CLIENT)
    m_mysqlHandle = mysql_init(nullptr);
    if (!m_mysqlHandle) {
        Logger::error("MySQL initialization failed.");
        throw DatabaseException("mysql_init failed to allocate handle.");
    }

    // Set connection timeout
    unsigned int timeout = static_cast<unsigned int>(m_config.timeoutSeconds);
    mysql_options(static_cast<MYSQL*>(m_mysqlHandle), MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

    // Auto-reconnect option
    bool reconnect = true;
    mysql_options(static_cast<MYSQL*>(m_mysqlHandle), MYSQL_OPT_RECONNECT, &reconnect);

    if (!mysql_real_connect(static_cast<MYSQL*>(m_mysqlHandle),
                            m_config.host.c_str(),
                            m_config.username.c_str(),
                            m_config.password.c_str(),
                            m_config.database.c_str(),
                            m_config.port,
                            nullptr,
                            CLIENT_MULTI_STATEMENTS)) {
        std::string err = mysql_error(static_cast<MYSQL*>(m_mysqlHandle));
        Logger::error("Failed to connect to MySQL database (" + m_config.database + "): " + err);
        mysql_close(static_cast<MYSQL*>(m_mysqlHandle));
        m_mysqlHandle = nullptr;
        m_connected = false;
        throw DatabaseException("MySQL Connection Error: " + err);
    }

    // Enforce UTF-8 communication
    mysql_set_character_set(static_cast<MYSQL*>(m_mysqlHandle), "utf8mb4");
    m_connected = true;
    Logger::info("Connected to MySQL Server 8.0 [" + m_config.database + "@" + m_config.host + "]");
    return true;
#else
    m_connected = false;
    Logger::error("Fatal Database Configuration: USE_MYSQL_CLIENT is not defined. MySQL 8.0 client library is strictly required.");
    throw DatabaseException("Fatal: Only genuine MySQL 8.0 database engine is supported.");
#endif
}

void Database::disconnect() {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
#if defined(USE_MYSQL_CLIENT)
    if (m_mysqlHandle) {
        mysql_close(static_cast<MYSQL*>(m_mysqlHandle));
        m_mysqlHandle = nullptr;
    }
#endif
    m_connected = false;
}

bool Database::isConnected() const { return m_connected; }

std::string Database::escapeString(const std::string& str) {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
#if defined(USE_MYSQL_CLIENT)
    if (m_connected && m_mysqlHandle) {
        std::vector<char> buffer(str.length() * 2 + 1);
        unsigned long len = mysql_real_escape_string(
            static_cast<MYSQL*>(m_mysqlHandle),
            buffer.data(),
            str.c_str(),
            static_cast<unsigned long>(str.length())
        );
        return std::string(buffer.data(), len);
    }
#endif
    std::string result;
    for (char c : str) {
        if (c == '\'') result += "\\'";
        else if (c == '\"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else result += c;
    }
    return result;
}

QueryResult Database::executeQuery(const std::string& sql) {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
    if (!m_connected) {
        connect();
    }

    QueryResult result;
#if defined(USE_MYSQL_CLIENT)
    if (mysql_query(static_cast<MYSQL*>(m_mysqlHandle), sql.c_str()) != 0) {
        std::string err = mysql_error(static_cast<MYSQL*>(m_mysqlHandle));
        Logger::error("SQL Query Error: " + err + " | SQL: " + sql);
        throw DatabaseException("Query execution failed: " + err);
    }

    MYSQL_RES* res = mysql_store_result(static_cast<MYSQL*>(m_mysqlHandle));
    if (res) {
        int numFields = mysql_num_fields(res);
        MYSQL_FIELD* fields = mysql_fetch_fields(res);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res))) {
            QueryRow qrow;
            unsigned long* lengths = mysql_fetch_lengths(res);
            for (int i = 0; i < numFields; ++i) {
                std::string colName = fields[i].name;
                std::string val = (row[i] && lengths[i] > 0) ? std::string(row[i], lengths[i]) : "";
                qrow.set(colName, val);
            }
            result.addRow(qrow);
        }
        mysql_free_result(res);
    }
#endif
    return result;
}

int Database::executeUpdate(const std::string& sql) {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
    if (!m_connected) {
        connect();
    }

#if defined(USE_MYSQL_CLIENT)
    if (mysql_query(static_cast<MYSQL*>(m_mysqlHandle), sql.c_str()) != 0) {
        std::string err = mysql_error(static_cast<MYSQL*>(m_mysqlHandle));
        Logger::error("SQL Update Error: " + err + " | SQL: " + sql);
        throw DatabaseException("Update execution failed: " + err);
    }
    return static_cast<int>(mysql_affected_rows(static_cast<MYSQL*>(m_mysqlHandle)));
#else
    throw DatabaseException("Cannot execute SQL update: MySQL 8.0 client is not enabled.");
#endif
}

long long Database::getLastInsertId() {
    std::lock_guard<std::recursive_mutex> lock(m_dbMutex);
#if defined(USE_MYSQL_CLIENT)
    if (m_mysqlHandle) {
        return static_cast<long long>(mysql_insert_id(static_cast<MYSQL*>(m_mysqlHandle)));
    }
#endif
    return 0;
}

void Database::beginTransaction() {
    executeUpdate("START TRANSACTION;");
}

void Database::commit() {
    executeUpdate("COMMIT;");
}

void Database::rollback() {
    executeUpdate("ROLLBACK;");
}

bool Database::testConnection() {
    try {
        if (!m_connected) connect();
        QueryResult res = executeQuery("SELECT 1 AS test_val;");
        return (!res.empty() && res[0].getInt("test_val") == 1);
    } catch (...) {
        return false;
    }
}
