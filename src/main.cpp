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
#include "logging.hpp"
#include "commands.hpp"
#include <random>
#include <sstream>
#include "base64.hpp"

namespace aelliptic {
    TgBot::Bot* bot;
    bool stop;
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
    log::init("bot.log");
    log::info("Initializing bot and registering commands");
    aelliptic::stop = false;
    TgBot::Bot _bot(argv[1]);
    bot = &_bot;
    std::string shutdown_token;
    { // Creating shutdown token
        std::random_device device;
        unsigned x = 0;
        std::stringstream ss;
        for (int i = 0; i < 8; i++) {
            x = device();
            ss << x;
        }
        shutdown_token = ss.str();
        shutdown_token = base64_encode(reinterpret_cast<const unsigned char*> 
                                       (shutdown_token.c_str()), 
                                       shutdown_token.length());
        std::cout << "Generated shutdown token: " << shutdown_token;
    }
    commands::register_commands();
    _bot.getEvents().onCommand("shutdown", 
    [&shutdown_token, &_bot](TgBot::Message::Ptr message) {
        std::stringstream ss;
        ss << "/shutdown " << shutdown_token;
        if (shutdown_token == ss.str()) {
            aelliptic::stop = true;
            _bot.getApi().sendMessage(message->chat->id, 
                                      "Bot will shut down after any event");
        }
    });
    try {
        std::string str = "Bot username: " + _bot.getApi().getMe()->username;
        log::info(str.c_str());
        TgBot::TgLongPoll longPoll(_bot);
        while (true) {
            longPoll.start();
            if (aelliptic::stop) break;
        }
    } catch (TgBot::TgException& e) {
        log::error("Exception occurred in bot poll loop!");
        log::error(e.what());
    }
    log::close();
    return 0;
}
