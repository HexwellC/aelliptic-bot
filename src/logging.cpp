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
#include "logging.hpp"
#include <iostream>
#include <ctime>
#include <mutex>
#include <iomanip>

namespace aelliptic {
    namespace log {
        std::mutex _log_mutex;
        std::ofstream *file;

        void init(const char* filename) {
            file = new std::ofstream();
            file->open(filename);
            if (file->fail()) {
                std::cerr << "Failed to initialize logging! The bot will exit."
                          << std::endl;
            }
        }

        void close() {
            _log_mutex.lock();
            if (!file->fail()) {
                file->close();
            }
            delete file;
            _log_mutex.unlock();
        }

        void log(const char* level, const char* msg) {
            static std::time_t time;
            static std::tm tm;
            time = std::time(nullptr);
            tm = *std::gmtime(&time);
            std::cout << std::put_time(&tm, "%F %T")
                      << " [" << level << "]: " << msg << std::endl;
            _log_mutex.lock();
            *file << std::put_time(&tm, "%F %T")
                  << " [" << level << "]: " << msg << std::endl;
            _log_mutex.unlock();
        }
    }
};
