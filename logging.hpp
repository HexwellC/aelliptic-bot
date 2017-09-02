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
#ifndef AELLIPTIC_BOT_OS_H
#define AELLIPTIC_BOT_OS_H
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <exception>
#include <string>
#include <fstream>

namespace aelliptic {
    namespace log {
        bool failbit = false;
        void init();
        void close();
        inline void trace(const std::string& msg);
        inline void info(const std::string& msg);
        inline void warn(const std::string& msg);
        inline void error(const std::exception& ex);
        inline void error(const std::exception& ex, const std::string& msg);
        void log(const std::string& level, const std::string& msg);
    }
}
#endif //AELLIPTIC_BOT_OS_H
