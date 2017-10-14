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
#pragma once

#include <tgbot/tgbot.h>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace aelliptic {
    extern TgBot::Bot* bot;
    
    inline 
    std::vector<std::string> tokenize(const TgBot::Message::Ptr& message) {
        std::vector<std::string> tokens;
        boost::trim(message->text);
        boost::split(tokens, message->text, boost::is_any_of("\t "), 
                     boost::token_compress_on);
        return tokens;
    }
}
