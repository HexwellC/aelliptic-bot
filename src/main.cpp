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
#include <signal.h>
#include <iostream>
#include <sstream>
#include "database.hpp"
#include "logging.hpp"
#include "irc_client.hpp"

static bool running;

using namespace aelliptic;

void shutdown(int signal) {
    running = false;
}

int main(int argc, char** argv) {
    std::cout << "AElliptic Bot  Copyright (C) 2017  HexwellC\nThis program "
            "comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and "
            "you are welcome to redistribute it\nunder certain conditions;"
              << std::endl;
    std::cout << "See LICENSE file for more details." << std::endl;
    if (argc < 3) {
        std::cerr << "Usage: ./aelliptic-bot <host> <port> [nick] [user]" 
                  << std::endl;
        return 1;
    }
    char* host = argv[1];
    int port = std::atoi(argv[2]);
    std::string nick("AEllipticBot");
    std::string user("AEllipticBot");
    if (argc >= 4) nick = argv[3];
    if (argc >= 5) user = argv[4];
    log::init("bot.log");
    log::info("Initializing bot and registering commands");
    database::init();
    irc::IRCClient client;
    {
        std::stringstream s;
        s << "Connecting to server: " << host << ":" << port;
        log::info(s.str());
    }
    if (client.connect(host, port)) {
        {
            std::stringstream s;
            s << "Logging in as " << nick << "(" << user << ")";
            log::info(s.str());
        }
        if (client.login(nick, user)) {
            log::info("Successfully logged in.");
            signal(SIGINT, shutdown);
            running = true;
            client.send_raw("JOIN #ungovernable");
            while (client.is_connected() && running) client.receive();
            running = false;
        }
        if (client.is_connected()) {
            client.disconnect();
        }
    }
    shutdown(0);
    aelliptic::database::free();
    log::close();
    return 0;
}
