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
#include "start.hpp"
AECOMM_NS
    void start(TgBot::Message::Ptr message) {
        bot->getApi().sendMessage(message->chat->id, "AElliptic Bot v1.0\n"
                "Copyright (C) HexwellC 2017, GNU GPLv3, Source code:\n"
                "https://github.com/HexwellC/aelliptic-bot/\n"
                "\n"
                "Currently I can do almost nothing.");
    }
AECOMM_NS_END
