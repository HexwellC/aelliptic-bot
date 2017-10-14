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

#include "base64.hpp"
#include "../base64.hpp"
#include <vector>
#include <boost/algorithm/string.hpp>

namespace aelliptic { namespace commands {
   
    static inline void usage(uint64_t id) {
        bot->getApi().sendMessage(id, 
                                 "Syntax:\n`/base64 encode <string>`\n"
                                 "`/base64 decode <base64_string>`",
                                 true, 0, TgBot::GenericReply::Ptr(),
                                 "Markdown");
    }
    
    void base64(TgBot::Message::Ptr message) {
        std::vector<std::string> tokens = tokenize(message);
        if(tokens.size() < 3) { 
            usage(message->chat->id);
            return;
        }
        std::string response;
        if(tokens[1] == "decode") {
            response = base64_decode(tokens[2]);
        } else if (tokens[1] == "encode") {
            response = base64_encode(reinterpret_cast
                                    <const unsigned char*>(tokens[2].c_str()), 
                                     tokens[2].length());
        } else {
            usage(message->chat->id);
            return;
        }
        try {
            bot->getApi().sendMessage(message->chat->id, response);
        } catch (...) { 
            bot->getApi().sendMessage(message->chat->id, 
                                      "ERROR: Invalid input");
        }
    }
}}
