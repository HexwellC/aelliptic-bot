//
// Created by hexwellc on 9/3/17.
//
#include "bot.hpp"

namespace aelliptic {
    Bot::Bot(char* token) : _bot(token) { }

    Bot::~Bot() { delete _bot; }

    TgBot::Bot* Bot::operator->() {
        return &_bot;
    }

    Bot bot;
}
