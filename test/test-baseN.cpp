#include <utility>

#include <base/baseN.hpp>
#include <base/hex.hpp>
#include <gtest/gtest.h>

using namespace baseN;

static const char b58digits[] =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

static const int8_t b58map[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1, -1, -1,
    -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
    -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //
};

TEST(baseN, isValid)
{
    std::vector<std::pair<bool, std::string>> tests = {
        {true, "123"},
        {false, "@#$"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, isValid(it.second, b58map));
}
TEST(baseN, encode)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"Ky", "044c"},
        {"KyK", "f94a"},
        {"KyKX", "387ae2"},
        {"KyKXa", "0ccbd755"},
        {"KyKXaa", "02e62ec963"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, encode(hex::decode(it.second), 58, b58digits));
}
TEST(baseN, encode_1e3)
{
    std::vector<uint8_t> data(1e3);
    std::fill(data.begin(), data.end(), 1);
    encode(data, 58, b58digits);
}
TEST(baseN, decode)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"Ky", "044c"},
        {"KyK", "f94a"},
        {"KyKX", "387ae2"},
        {"KyKXa", "0ccbd755"},
        {"KyKXaa", "02e62ec963"},
    };
    for (auto it : tests)
        EXPECT_EQ(hex::encode(decode(it.first, 58, b58digits, b58map)), it.second);
}
TEST(baseN, decode_1e3)
{
    std::string str(1e3, '2');
    decode(str, 58, b58digits, b58map);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}