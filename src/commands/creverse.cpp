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

#include "creverse.hpp"
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <codecvt>
#include <cassert>
#include <locale>

namespace aelliptic::commands {
   
    static inline void usage(uint64_t id) {
        bot->getApi().sendMessage(id, 
                                 "Syntax:\n`/reverse <string>`\n",
                                 true, 0, TgBot::GenericReply::Ptr(),
                                 "Markdown");
    }
    
    void reverse(const TgBot::Message::Ptr& message) {
		std::vector<std::string> tokens = tokenize(message->text);
		if(tokens.size() < 2){
			usage(message->chat->id);
			return;
		}

		std::string text;
		for(unsigned i = 1; i < tokens.size(); ++i){
			text += tokens[i];
			text += " ";
		}

		std::u16string u16_conv = std::wstring_convert<
				std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(text);

		std::u16string result(u16_conv.rbegin(), u16_conv.rend());

		std::string u8_result = std::wstring_convert<
        		std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(result);

		bot->getApi().sendMessage(message->chat->id, u8_result, 
		 						true, 0, TgBot::GenericReply::Ptr(),
                                "Markdown");

    }
}
 
