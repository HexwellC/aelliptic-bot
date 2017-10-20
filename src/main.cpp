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
#include <csignal>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace aelliptic {
    TgBot::Bot* bot;
    bool stop;
}

using namespace aelliptic;

void sigint(int) {
    if (stop) {
        log::error("SIGINT caught, forcing exit with error");
        log::close();
        std::remove("SHUTDOWN_KEY");
        std::exit(1);
    }
    log::warn("SIGINT caught, bot will exit after any network event");
    stop = true;
}

int main() {
    std::cout << "AElliptic Bot  Copyright (C) 2017  HexwellC\nThis program "
            "comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and "
            "you are welcome to redistribute it\nunder certain conditions;"
              << std::endl;
    std::cout << "See LICENSE file for more details." << std::endl;

    std::string token;
    bool show_shutdown_token = false;

    {
        std::ifstream conf_stream("bot.conf");
        
        if (!conf_stream.good()) {
            conf_stream.close();
            std::ofstream new_conf("bot.conf");
            new_conf << "token=INSERT_TOKEN\n";
            new_conf << "show-shutdown-key=true";
            new_conf << std::endl;
            std::cout << "Created new config, please change token "
                      << "and restart application!" << std::endl;
            return 0;
        }
        
        std::string line;
        std::vector<std::string> pairs;
        int n_line = 0;
        while (std::getline(conf_stream, line)) {
            n_line++;
            if (line[0] == '#') continue;
            boost::split(pairs, line, boost::is_any_of("="));
        
            if (pairs[0] == "token") {
                token = pairs[1];
            } else if (pairs[0] == "show-shutdown-key") {
                if (pairs[1] == "true" || pairs[1] == "1") {
                    show_shutdown_token = true;
                } else if (pairs[1] == "false" || pairs[1] == "0") {
                    // Do nothing
                } else {
                    std::cerr << "\nInvalid config value, line: " << n_line
                              << ": " << line << std::endl;
                }
            } else {
                std::cerr << "Unknown option, line: " << n_line << ": "
                         << line << std::endl;
            }
        }

        if (n_line < 1) {
            std::cerr << "\nBot config is empty!" << std::endl;
        }
    }

    if (token.empty() || token == "INSERT_TOKEN") {
        std::cerr << "\nNo token supplied!" << std::endl;
        return 1;
    }

    log::init("bot.log");
    log::info("Initializing bot and registering commands");
    TgBot::Bot _bot(token);
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
        if (show_shutdown_token) std::cout << "Generated shutdown token: "
                                           << shutdown_token << std::endl;
        std::ofstream("SHUTDOWN_KEY") << shutdown_token << std::endl;
    }

    _bot.getEvents().onCommand("shutdown", 
    [&shutdown_token](TgBot::Message::Ptr message) {
        if (message->text.substr(10) == shutdown_token) {
            aelliptic::stop = true;
        }
    });

    commands::register_commands();

    try {
        TgBot::TgLongPoll longPoll(_bot);
        std::signal(SIGINT, sigint);
        aelliptic::stop = false;

        log::info("Started, listening for updates");
        while (true) {
            longPoll.start();
            if (aelliptic::stop) break;
        }
    } catch (TgBot::TgException& e) {
        log::error("Exception occurred in bot poll loop!");
        log::error(e.what());
    }

    log::close();
    std::remove("SHUTDOWN_KEY");
    return 0;
}
