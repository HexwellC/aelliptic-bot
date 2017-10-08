// AElliptic Bot - bot for Ungovernable IRC
// Copyright (C) 2017  HexwellC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "database.hpp"
#include "logging.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <mutex>

namespace aelliptic { namespace database {
    static std::mutex db_mutex;
    sqlite3 *database = nullptr;
    
    
    std::vector<std::string> _result;
    
    static int _exec_callback(void*, int argc, char** argv, char** colName) {
        std::vector<std::string> results(argc);
        for (int i = 0; i < argc; i++) results.push_back(argv[i]);
        _result = std::move(results);
        return 0;
    }
    
    std::vector<std::string>&& exec(const char* sql) {
        char* err;
        db_mutex.lock();
        int rc = sqlite3_exec(database, sql, _exec_callback, nullptr, &err);
        db_mutex.unlock();
        if (rc) {
            log::error("Database error!");
            log::error(err);
            _result.clear();
        }
        return std::move(_result);
    }
    
    int init() {
        if (database != nullptr) return 0;
        int rc = sqlite3_open("database.db", &database);
        if (rc) {
            log::error("Failed to initialize database!");
            log::error(sqlite3_errmsg(database));
        } else {
            exec("CREATE TABLE IF NOT EXISTS bot(uid, money, pidors)");
        }
        return rc;
    }

    void free() {
        sqlite3_close(database);
    }
}}
