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
#include <iostream>
#include <tgbot/tgbot.h>
#include "logging.hpp"
#include "commands.hpp"

namespace aelliptic {
    TgBot::Bot* bot;
}

using namespace aelliptic;

int main(int argc, char** argv) {
    std::cout << "AElliptic Bot  Copyright (C) 2017  HexwellC\nThis program "
            "comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and "
            "you are welcome to redistribute it\nunder certain conditions;"
              << std::endl;
    std::cout << "See LICENSE file for more details." << std::endl;
    if (argc < 2) {
        std::cerr << "No token supplied!" << std::endl;
        return 1;
    }
    // WatchDog is also a RAII wrapper for everything that needs to be closed
    // on application exit.
    log::init("bot.log");
    log::info("Initializing bot and registering commands");
    TgBot::Bot _bot(argv[1]);
    bot = &_bot;
    commands::_registerCommands();
    try {
        std::string str = "Bot username: " + _bot.getApi().getMe()->username;
        log::info(str.c_str());
        TgBot::TgLongPoll longPoll(_bot);
        while (true) {
            log::trace("Long poll started");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        log::error("Exception occurred in bot poll loop!");
        log::error(e.what());
    }
    log::close();
    return 0;
}
