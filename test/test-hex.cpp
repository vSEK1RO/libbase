#include <base/hex.hpp>
#include <gtest/gtest.h>

using namespace hex;

TEST(hex, encode)
{
    std::vector<uint8_t> data = { 0x74, 0x65, 0x73, 0x74 };
    EXPECT_EQ("74657374", encode(data));
}
TEST(hex, encode_1e6)
{
    std::vector<uint8_t> data(1e6);
    encode(data);
}
TEST(hex, decode)
{
    std::vector<uint8_t> data = { 0x61, 0x6e, 0x6f };
    EXPECT_EQ(decode("616e6f"), data);
}
TEST(hex, decode_1e6)
{
    std::string str(1e6, '0');
    decode(str);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}