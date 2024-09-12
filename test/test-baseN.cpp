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
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
    -1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
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
};

TEST(baseN, isValid)
{
    EXPECT_EQ(true, isValid("123", b58map));
    EXPECT_EQ(false, isValid("@#$", b58map));
}
TEST(baseN, encode)
{
    EXPECT_EQ("Ky", encode(hex::decode("044c"), 58, b58digits));
    EXPECT_EQ("KyK", encode(hex::decode("f94a"), 58, b58digits));
    EXPECT_EQ("KyKX", encode(hex::decode("387ae2"), 58, b58digits));
    EXPECT_EQ("KyKXa", encode(hex::decode("0ccbd755"), 58, b58digits));
    EXPECT_EQ("KyKXaa", encode(hex::decode("02e62ec963"), 58, b58digits));
}
// TEST(baseN, encode_1e6)
// {
//     std::vector<uint8_t> data(1e6);
//     encode(data);
// }
TEST(baseN, decode)
{
    EXPECT_EQ(hex::encode(decode("Ky", 58, b58digits, b58map)), "044c");
    EXPECT_EQ(hex::encode(decode("KyK", 58, b58digits, b58map)), "f94a");
    EXPECT_EQ(hex::encode(decode("KyKX", 58, b58digits, b58map)), "387ae2");
    EXPECT_EQ(hex::encode(decode("KyKXa", 58, b58digits, b58map)), "0ccbd755");
    EXPECT_EQ(hex::encode(decode("KyKXaa", 58, b58digits, b58map)), "02e62ec963");
}
// TEST(baseN, decode_1e6)
// {
//     std::string str(1e6, '0');
//     decode(str);
// }

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}