#include <gtest/gtest.h>
#include "base64.hpp"

using namespace aelliptic;
inline auto base64_encode_wrapper(const std::string& input) {
    return base64_encode(reinterpret_cast<const unsigned char*>(input.data()), input.size());
}

inline auto base64_decode_wrapper(const std::string& input) {
    return base64_decode(input);
}

TEST(base64_tests, encode) {
    ASSERT_EQ(base64_encode_wrapper("1234567890"), "MTIzNDU2Nzg5MA==");
    ASSERT_EQ(base64_encode_wrapper(""),           "");
}

TEST(base64_tests, decode) {
    ASSERT_EQ(base64_decode_wrapper("MTIzNDU2Nzg5MA=="), "1234567890");
    ASSERT_EQ(base64_decode_wrapper(""),                 "");
}
