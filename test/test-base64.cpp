#include <base/base64.hpp>
#include <base/hex.hpp>
#include <gtest/gtest.h>

using namespace base64;

TEST(base64, isValid)
{
    EXPECT_TRUE(isValid("12=="));
    EXPECT_TRUE(isValid("123="));
    EXPECT_TRUE(isValid("1234"));

    EXPECT_FALSE(isValid("1==="));
    EXPECT_FALSE(isValid("?!*"));
}
TEST(base64, encode)
{
    EXPECT_EQ(encode(hex::decode("")), "");
    EXPECT_EQ(encode(hex::decode("04a504a5")), "BKUEpQ==");
    EXPECT_EQ(encode(hex::decode("04a504a500")), "BKUEpQA=");
    EXPECT_EQ(encode(hex::decode("04a504a50000")), "BKUEpQAA");
}
TEST(base64, encode_1e7)
{
    std::vector<uint8_t> data(1e7);
    encode(data);
}
// TEST(base64, decode)
// {
// }
// TEST(base64, decode_1e6)
// {
//     std::string str(1e6, '0');
//     decode(str);
// }

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}