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
        /// Used in WatchDog, do not use in other places
        void init(const char* filename);

        /// Used in WatchDog, do not use in other places
        void close();

        /// General logger
        /**
         * Shouldn't actually be used, see info, warn and error levels.
         * @param level Logging level string, often called "flag"
         * @param msg Message to log
         */
        void log(const char* level, const char* msg);

        /// Informative logger
        /**
         * Simply logs a message with INFO flag.
         * @param msg Message to log
         */
        inline void info(const char* msg) { log("INFO", msg); }

        /// Warning logger
        /**
         * Logs a warning. Almost no difference with info level, except flag.
         * @param msg Message to log
         */
        inline void warn(const char* msg) { log("WARN", msg); }

        /// Error logger
        /**
         * Logs exception message.
         * Note: in most cases, you probably want a warn logging level.
         * Seriously.
         * @param ex Error to log
         */
        inline void error(const char* ex) { log("ERROR", ex); }

#ifndef AE_NO_TRACE_LOG
        /// Trace logger
        /**
        * Tracing level is logged only to console. It can be disabled by
        * defining AE_NO_TRACE_LOG
        * @param msg Message to log
        */
        void trace(const char* msg);
#else
        inline void trace(const std::string& msg) { }
#endif
    }
}
#endif //AELLIPTIC_BOT_OS_H
