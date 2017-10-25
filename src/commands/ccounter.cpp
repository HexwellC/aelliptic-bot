// AElliptic Bot - bot for Autistic Epilepsy Foundation chat in Telegram
// Copyright (C) 2017  inagen & foxcpp
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

std::map<int64_t, int> counter;
int max;

int bin_to_dec(const std::string& bin){
	int result = 0;
	int i = 1;
	
	for(auto it = bin.cend(); it != bin.cbegin(); it++){
		if(*it == '1')
			result += i;
		i *=2;
	}
	return result;
}

std::string dec_to_bin(int dec)
{
	std::bitset< sizeof(int) * 8 > bin(dec);
	return bin.to_string();
}

bool is_binary(const std::string& text)
{
	for(auto it = text.cbegin(); it != text.cend(); it++){
		if(*it != '0' || *it != '1')
			return false;
	}
	return true;
}

namespace aelliptic::commands {

	void counter_message(const TgBot::Message::Ptr& message){
		std::vector<std::string> tokens = tokenize(message->text);

		log::info("ПРИШЛО ЧИСЛО");
		/*if(counter.empty())
			return;
*/
		if(is_binary(tokens[0])) {

			log::info("ЧИСЛО	БИНАРНОЕ");
			std::srand( time(0) );
			auto it = counter.find(message->chat->id);
			if (it == counter.end())
				return;

			int& num = it->second;

			if(num == max){
				log::info("ИГРА ОКОНЧЕНА");
				bot->getApi().sendMessage(message->chat->id, "Game over!");
				counter.erase(counter.find(message->chat->id));
				return;
			}
			
			if( bin_to_dec(tokens[0]) != num + 1){
				log::info("ЕБУЧEE НЕВЕРНО!");
				bot->getApi().sendMessage(message->chat->id, "wrong!");
				return;
			}

			if(std::rand() % 2 == 1){
				log::info("ПРИСЫЛАЮ ЧИСЛО");
				bot->getApi().sendMessage(message->chat->id, dec_to_bin(num + 2));
				return;
			}

			
		}
	}

	void counter_command(const TgBot::Message::Ptr& message) {
		log::info("ПРИШЛА ЕБУЧАЯ КОМАНДА");
		bot->getApi().sendMessage(message->chat->id, "Game started!");
		bot->getApi().sendMessage(message->chat->id, "00000000");
		std::vector<std::string> tokens = tokenize(message->text);
		counter.emplace(message->chat->id, 0);
	}

}