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

#include "ccounter.hpp" 
#include <map>
#include <bitset>
#include <ctime>
#include "logging.hpp"

std::map<int64_t, unsigned> counter;
std::map<int64_t, unsigned> max;

unsigned bin_to_dec(const std::string& bin) {
	std::bitset<sizeof(unsigned) * 8> set{bin};
	return set.to_ulong();
}

std::string dec_to_bin(unsigned dec) {
	std::bitset<sizeof(unsigned) * 8> bin(dec);
	return bin.to_string();
}

bool is_binary(const std::string& text) {
    if (text.empty()) return false;
	for (char ch : text) {
		if (ch != '0' && ch != '1') return false;
	}
	return true;
}

namespace aelliptic::commands {
	void counter_message(const TgBot::Message::Ptr& message) {
		std::vector<std::string> tokens = tokenize(message->text);

		if (is_binary(tokens[0])) {
			log::info("Looks like binary number...");
			std::srand(std::time(nullptr));
			auto it = counter.find(message->chat->id);
			if (it == counter.end())
				return;

			unsigned& num = it->second;

            log::info("Counter is...");
            log::info(dec_to_bin(num).c_str());

			if (num >= max.at(message->chat->id)) {
				log::info("Game over");
				bot->getApi().sendMessage(message->chat->id, "Game over!");
				counter.erase(counter.find(message->chat->id));
				return;
			}

			if (bin_to_dec(tokens[0]) != num + 1) {
				log::info("Wrong number.");
                log::info("Should be...");
                log::info(dec_to_bin(num + 1).c_str());
                log::info("But is...");
                log::info(dec_to_bin(bin_to_dec(tokens[0])).c_str());
				bot->getApi().sendMessage(message->chat->id, "wrong!");
				return;
			}

			if (std::rand() % 2 == 1) {
				log::info("Responding...");
				bot->getApi().sendMessage(message->chat->id, dec_to_bin(num + 2));
                log::info("Updating counter...");
                log::info((std::string(" Was: ") + dec_to_bin(num)).c_str());
                ++num;
                log::info((std::string(" Now: ") + dec_to_bin(num)).c_str());
			}

            log::info("Updating counter...");
            log::info((std::string(" Was: ") + dec_to_bin(num)).c_str());
            ++num;
            log::info((std::string(" Now: ") + dec_to_bin(num)).c_str());
		}
	}

	void counter_command(const TgBot::Message::Ptr& message) {
		log::info("Started a game.");
		bot->getApi().sendMessage(message->chat->id, "Game started!");
		bot->getApi().sendMessage(message->chat->id, "00000000000000000000000000000000");
        auto tokens = tokenize(message->text);
		counter.emplace(message->chat->id, 0);
        max.emplace(message->chat->id, std::stoi(tokens[1]));
	}

}
