#include "database.hpp"
#include "logging.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <mutex>
#include <vector>

namespace aelliptic { namespace database {
    static std::mutex db_mutex;
    sqlite3 *database = nullptr;
    
    int init() {
        if (database != nullptr) return 0;
        return sqlite3_open("database.db", &database);
    }
    
    std::vector<std::string> _result;
    
    int _exec_callback(void*, int argc, char** argv, char** colName) {
        std::vector<std::string> results(argc);
        for (int i = 0; i < argc; i++) results.push_back(argv[i]);
        _result = std::move(results);
        return 0;
    }
    
    std::vector<std::string>&& exec(const char* sql) {
        char* err;
        int rc = sqlite3_exec(database, sql, _exec_callback, 0, &err);
        if (rc) {
            log::error("Database error!");
            log::error(err);
            return std::move(std::vector<std::string>());
        }
        return std::move(_result);
    }
    
    void free() {
        sqlite3_close(database);
    }
}}
