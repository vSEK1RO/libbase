#include <utility>

#include <basen/base58.hpp>
#include <basen/baseN.hpp>
#include <basen/hex.hpp>
#include <gtest/gtest.h>

using namespace baseN;

TEST(baseN, digitsMap)
{
    int8_t map[256];
    digitsMap(base58::digits, 58, map);
    EXPECT_TRUE(std::equal(map, map + 256, base58::map));

    EXPECT_THROW(digitsMap("11", 2, map), std::logic_error);
}
TEST(baseN, isValid)
{
    std::vector<std::pair<bool, std::string>> tests = {
        {true, "123"},
        {false, "@#$"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, isValid(it.second, base58::map));
}
TEST(baseN, sizeEncoded)
{
    std::vector<std::pair<size_t, std::string>> tests = {
        {6, "12341234"},
        {5, "00000000"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, sizeEncoded(hex::decode(it.second), 58));
}
TEST(baseN, sizeDecoded)
{
    std::vector<std::pair<size_t, std::string>> tests = {
        {3, "qwer"},
        {5, "1111"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, sizeDecoded(it.second, 58, base58::digits));
}
std::vector<std::pair<std::string, std::string>> tests = {
    {"", ""},
    {"Ky", "044c"},
    {"KyK", "f94a"},
    {"KyKX", "387ae2"},
    {"KyKXa", "0ccbd755"},
    {"KyKXaa", "02e62ec963"},
    {"111KyKX", "000000387ae2"},
    {"4uqWDRyJZUpS6KKwLAiitndmv7TPFt2bfxVVfhJhgTn3Rh6aQtGHQY6PhhNDpCwSNU8a",
     "057902f9cebebb68879911002aae743280140a78c4a077405b057902f9cebebb68879911002aae743280140a78c4a077405b"},
};
TEST(baseN, encode)
{
    for (auto it : tests)
        EXPECT_EQ(it.first, encode(hex::decode(it.second), 58, base58::digits));
}
TEST(baseN, encode_1e3)
{
    std::vector<uint8_t> data(1e3);
    std::fill(data.begin(), data.end(), 1);
    encode(data, 58, base58::digits);
}
TEST(baseN, decode)
{
    for (auto it : tests)
        EXPECT_EQ(hex::encode(decode(it.first, 58, base58::digits, base58::map)), it.second);
}
TEST(baseN, decode_1e3)
{
    std::string str(1e3, '2');
    decode(str, 58, base58::digits, base58::map);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}