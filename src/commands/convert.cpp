// AElliptic Bot - bot for Autistic Epilepsy Foundation chat in Telegram
// Copyright (C) 2017  inagen
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

#include "convert.hpp"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <string>

double convert_temp(std::string in_what, std::string data){

    if(in_what == "C/F")
        return atof(data.c_str()) * 1.8 + 32;
    else if(in_what == "F/C")
        return (atof(data.c_str()) - 32) / 1.8;
    else if(in_what == "C/K")
        return atof(data.c_str())  - 273.15;
    else if(in_what == "K/C")
        return atof(data.c_str()) + 273.15;
    else if(in_what == "F/K")
        return (atof(data.c_str()) - 32) / 1.8 - 273;
    else if(in_what == "K/F")
        return (atof(data.c_str()) * 1.8 + 32) + 273;
    
    throw std::logic_error("description");
}


namespace aelliptic { namespace commands {

    static void usage_iw(uint64_t id) {
    bot->getApi().sendMessage(id, 
                             "Syntax:\n`/convert <what> <from>/<to> <data>`\n"
                             "Please enter correct data in <from>/<to> space"
                             "eg: F/C, C/F",
                             true, 0, TgBot::GenericReply::Ptr(),
                             "Markdown");
}

    static void convert_usage(uint64_t id) {
        bot->getApi().sendMessage(id, 
                                 "Syntax:\n`/convert <what> <from>/<to>"
                                 "<data>`\n",
                                 true, 0, TgBot::GenericReply::Ptr(),
                                 "Markdown");
    }

    
    void convert(TgBot::Message::Ptr message) {
        std::vector<std::string> tokens = tokenize(message);
        if(tokens.size() < 4) { 
            convert_usage(message->chat->id);
            return;
        }
        std::string response;
        if(tokens[1] == "temperature"){
            try {
                std::stringstream ss;
                ss << convert_temp(tokens[2], tokens[3]);
                response = ss.str();
            } catch(...) {
                usage_iw(message->chat->id);
                return;
            }
        } else {
            convert_usage(message->chat->id);
            return;
        }

        bot->getApi().sendMessage(message->chat->id, response);
    }
}}
