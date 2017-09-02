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
#ifndef AELLIPTIC_BOT_BOT_HPP
#define AELLIPTIC_BOT_BOT_HPP

#include <tgbot/Bot.h>

namespace aelliptic {
    class _Bot {
    public:
        _Bot() = default;
        _Bot(char* token);
        ~_Bot();
        TgBot::Bot *operator->();
    private:
        TgBot::Bot _bot;
    };
    extern _Bot bot;
}
#endif //AELLIPTIC_BOT_BOT_HPP