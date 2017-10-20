#include <gtest/gtest.h>
#include "bot.hpp"

using namespace aelliptic;

TEST(bot, tokenize) {
    std::vector<std::string> result = { "aaa", "bbb" };

    ASSERT_EQ(tokenize("aaa\t\tbbb"), result);
    ASSERT_EQ(tokenize(""),           std::vector<std::string>());
    ASSERT_EQ(tokenize("    \t\t"),   std::vector<std::string>());
}
