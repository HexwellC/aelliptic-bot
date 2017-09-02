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
#ifndef AELLIPTIC_BOT_WATCHDOG_HPP
#define AELLIPTIC_BOT_WATCHDOG_HPP
namespace aelliptic {
    /// Watcher-class for keeping eye on bot systems
    /**
     * Actually, this class is more than just a watcher. It is mainly ought
     * to be used as a RAII wrapper for everything that needs to be closed or
     * destructed with program itself, as the only reference to watchdog that
     * should exist is the reference in program main function.
     * In future, probably, this class will be used for something else. For
     * now, it is just a RAII wrapper for logging.
     */
    class WatchDog {
    public:
        explicit WatchDog(const char* logfile);

        ~WatchDog();
    };
}
#endif //AELLIPTIC_BOT_WATCHDOG_HPP
