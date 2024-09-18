#include <utility>

#include <base/base64.hpp>
#include <base/hex.hpp>
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
    {"BKUEpQA=", "04a504a500"},
    {"BKUEpQAA", "04a504a50000"},
};
TEST(base64, encode)
{
    for (auto it : tests)
        EXPECT_EQ(it.first, encode(hex::decode(it.second)));
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