// AElliptic Bot - bot for Autistic Epilepsy Foundation chat in Telegram
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
#include "logging.hpp"
#include <iostream>
#include <ctime>
#include <mutex>

namespace aelliptic {
    namespace log {
        std::mutex _log_mutex;
        std::ofstream *file;

        void init(const char* filename) {
            _log_mutex.lock();
            file = new std::ofstream();
            file->open(filename);
            failbit = file->fail();
            _log_mutex.unlock();
        }

        void close() {
            _log_mutex.lock();
            if (!file->fail()) {
                file->close();
            }
            delete file;
            _log_mutex.unlock();
        }

        void log(const std::string& level, const std::string& msg) {
            _log_mutex.lock();
            auto cur_time = std::ctime(nullptr);
            std::cout << cur_time << " [" << level << "]: " << msg;
            *file << cur_time << " [" << level << "]: " << msg;
            _log_mutex.unlock();
        }

        inline void trace(const std::string& msg) {
#ifndef AE_NO_TRACE_LOG
            std::cout << std::ctime(nullptr) << " [TRACE]: " << msg;
#endif
        }

        inline void info(const std::string& msg) { log("INFO", msg); }

        inline void warn(const std::string& msg) { log("WARN", msg); }

        inline void error(const std::exception& e) { log("ERROR", e.what()); }

        inline void error(const std::exception& e, const std::string& msg) {
            log("ERROR", msg);
            log("ERROR", e.what());
        }
    }
};
