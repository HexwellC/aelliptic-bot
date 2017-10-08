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
#include <cstring>
#include <fcntl.h>
#include "irc_socket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#define MAXDATASIZE 4096

namespace aelliptic { namespace irc {
    IRCSocket::IRCSocket() {
        if ((_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) 
            == INVALID_SOCKET) {
            log::error("Socket error: invalid socket!");
            std::exit(1);
        }
        int on = 1;
        if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, 
                       (char const*)&on, sizeof(on)) == -1) {
            log::error("Socket error: invalid socket!");
            std::exit(1);
        }
        fcntl(_socket, F_SETFL, O_NONBLOCK);
        fcntl(_socket, F_SETFL, O_ASYNC);
    }
    
    IRCSocket::~IRCSocket() {
        if (_connected) disconnect();
    }
    
    bool IRCSocket::connect(const char* host, int port) {
        hostent* he;

        if (!(he = gethostbyname(host))) {
            log::error("Couldn't resolve host!");
            log::error(host);
            return false;
        }

        sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr = *((const in_addr*)he->h_addr);
        memset(&(addr.sin_zero), '\0', 8);

        if (::connect(_socket, (sockaddr*)&addr, sizeof(addr)) 
            == SOCKET_ERROR) {
            log::error("Socket connection error!");
            log::error(host);
            close(_socket);
            return false;
        }

        _connected = true;
        return true;
    }
    
    void IRCSocket::disconnect() {
        if (_connected) {
            close(_socket);
            _connected = false;
            log::info("Disconnected!");
        }
    }
    
    bool IRCSocket::send(const char* data) {
        if ((!_connected) || (::send(_socket, data, strlen(data), 0) == -1)) 
            return false;
        return true;
    }

    std::string IRCSocket::receive() {
        char buffer[MAXDATASIZE];
        memset(buffer, 0, MAXDATASIZE);
        int bytes = recv(_socket, buffer, MAXDATASIZE - 1, 0);
        if (bytes > 0)
            return std::string(buffer);
        else
            disconnect();
        return "";
    }
}}
