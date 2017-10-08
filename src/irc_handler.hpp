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
#include "irc_client.hpp"

#define NUM_IRC_CMDS 26

namespace aelliptic { namespace irc {
    struct IRCCommandHandler {
        std::string command;
        void (IRCClient::*handler)(IRCMessage /*message*/);
    };

    extern IRCCommandHandler ircCommandTable[NUM_IRC_CMDS];

    inline int GetCommandHandler(std::string command) {
        for (int i = 0; i < NUM_IRC_CMDS; ++i)
            if (ircCommandTable[i].command == command) return i;

        return NUM_IRC_CMDS;
    }
}}
