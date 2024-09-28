#include <utility>

#include <basen/base64.hpp>
#include <basen/hex.hpp>
#include <gtest/gtest.h>

using namespace base64;

TEST(base64, isValid)
{
    std::vector<std::pair<bool, std::string>> tests = {
        {true, "12=="},
        {true, "123="},
        {true, "1234"},
        {false, "1==="},
        {false, "?!*"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, isValid(it.second));
}
std::vector<std::pair<std::string, std::string>> tests = {
    {"", ""},
    {"BKUEpQ==", "04a504a5"},
    {"aGVsbG8=", "68656c6c6f"},
    {"aGVsbG9v", "68656c6c6f6f"},
};
TEST(base64, encode)
{
    for (auto it : tests)
        EXPECT_EQ(it.first, encode(hex::decode(it.second)));

    std::vector<uint8_t> data = {0x74, 0x65, 0x73, 0x74};
    std::string str = "";
    EXPECT_THROW(encode(data.data(), std::numeric_limits<uint64_t>::max(), str.data(), str.size()), std::overflow_error);
    EXPECT_THROW(encode(data.data(), data.size(), str.data(), str.size()), std::length_error);
    EXPECT_NO_THROW(encode(data.data(), 0, str.data(), str.size()));
}
TEST(base64, encode_1e7)
{
    std::vector<uint8_t> data(1e7);
    encode(data);
}
TEST(base64, decode)
{
    for (auto it : tests)
        EXPECT_EQ(hex::encode(decode(it.first)), it.second);

    std::vector<uint8_t> data = {0x61, 0x6e, 0x6f};
    EXPECT_THROW(decode("FFF", 3, data.data(), data.size()), std::logic_error);
    EXPECT_THROW(decode("!@#!", 4, data.data(), data.size()), std::logic_error);
    EXPECT_THROW(decode("FF==", 2, data.data(), 0), std::length_error);
    EXPECT_NO_THROW(decode("", 0, data.data(), 0));
}
TEST(base64, decode_1e7)
{
    std::string str(1e7, '0');
    decode(str);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}