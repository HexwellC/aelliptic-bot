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
#include <iostream>
#include <algorithm>
#include <sstream>
#include "irc_client.hpp"
#include "irc_handler.hpp"

namespace aelliptic { namespace irc {
    std::vector<std::string> split(std::string const& text, char sep) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    }

    bool IRCClient::connect(char* host, int port) {
        return _socket.connect(host, port);
    }

    void IRCClient::disconnect() {
        _socket.disconnect();
    }

    bool IRCClient::send_irc(std::string data) {
        data.append("\n");
        return _socket.send(data.c_str());
    }

    bool IRCClient::login(std::string nick, std::string user, 
                          std::string password) {
        _nick = nick;
        _user = user;

        if (send_irc("HELLO")) {
            if (!password.empty() && !send_irc("PASS "+password))
                return false;
            if (send_irc("NICK " + nick))
                if (send_irc("USER " + user + " 8 * :Cpp IRC Client"))
                    return true;
        }
        
        return false;
    }

    void IRCClient::receive() {
        std::string buffer = _socket.receive();

        std::string line;
        std::istringstream iss(buffer);
        while(getline(iss, line)) {
            if (line.find("\r") != std::string::npos)
                line = line.substr(0, line.size() - 1);
            parse(line);
        }
    }

    void IRCClient::parse(std::string data) {
        std::string original(data);
        IRCCommandPrefix cmdPrefix;

        // if command has prefix
        if (data.substr(0, 1) == ":") {
            cmdPrefix.parse(data);
            data = data.substr(data.find(" ") + 1);
        }

        std::string command = data.substr(0, data.find(" "));
        std::transform(command.begin(), command.end(), 
                       command.begin(), towupper);
        if (data.find(" ") != std::string::npos)
            data = data.substr(data.find(" ") + 1);
        else
            data = "";

        std::vector<std::string> parameters;

        if (data != "") {
            if (data.substr(0, 1) == ":") parameters.push_back(data.substr(1));
            else {
                size_t pos1 = 0, pos2;
                while ((pos2 = data.find(" ", pos1)) != std::string::npos) {
                    parameters.push_back(data.substr(pos1, pos2 - pos1));
                    pos1 = pos2 + 1;
                    if (data.substr(pos1, 1) == ":") {
                        parameters.push_back(data.substr(pos1 + 1));
                        break;
                    }
                }
                if (parameters.empty()) parameters.push_back(data);
            }
        }

        if (command == "ERROR") {
            std::cout << original << std::endl;
            disconnect();
            return;
        }

        if (command == "PING") {
            std::cout << "Ping? Pong!" << std::endl;
            send_irc("PONG :" + parameters.at(0));
            return;
        }

        IRCMessage ircMessage(command, cmdPrefix, parameters);

        // Default handler
        int commandIndex = GetCommandHandler(command);
        if (commandIndex < NUM_IRC_CMDS) {
            IRCCommandHandler& cmdHandler = ircCommandTable[commandIndex];
            (this->*cmdHandler.handler)(ircMessage);
        } else if (_debug)
            std::cout << original << std::endl;

        // Try to call hook (if any matches)
        CallHook(command, ircMessage);
    }

    void IRCClient::hook_command(std::string command, 
                                 void (*function)(IRCMessage /*message*/, 
                                                  IRCClient* /*client*/)) {
        IRCCommandHook hook;

        hook.command = command;
        hook.function = function;

        _hooks.push_back(hook);
    }

    void IRCClient::CallHook(std::string command, IRCMessage message) {
        if (_hooks.empty()) return;

        for (std::list<IRCCommandHook>::const_iterator itr = _hooks.begin(); 
             itr != _hooks.end(); ++itr) {
            if (itr->command == command) {
                (*(itr->function))(message, this);
                break;
            }
        }
    }
}}

