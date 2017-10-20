#include <gtest/gtest.h>
#include "base64_test.hpp"
#include "tokenizer_test.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
