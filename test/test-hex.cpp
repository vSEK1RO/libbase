#include <utility>

#include <basen/Exception.hpp>
#include <basen/hex.hpp>
#include <gtest/gtest.h>

using namespace hex;

TEST(hex, isValid)
{
    std::vector<std::pair<bool, std::string>> tests = {
        {true, "1234"},
        {false, "!@/"},
    };
    for (auto it : tests)
        EXPECT_EQ(it.first, isValid(it.second));
}
TEST(hex, encode)
{
    std::vector<uint8_t> data = {0x74, 0x65, 0x73, 0x74};
    EXPECT_EQ("74657374", encode(data));

    std::string str = "";
    EXPECT_THROW(encode(data.data(), std::numeric_limits<size_t>::max(), str.data(), str.size()), basen::Exception);
    EXPECT_THROW(encode(data.data(), data.size(), str.data(), str.size()), basen::Exception);
    EXPECT_NO_THROW(encode(data.data(), 0, str.data(), str.size()));
}
TEST(hex, encode_1e7)
{
    std::vector<uint8_t> data(1e7);
    encode(data);
}
TEST(hex, decode)
{
    std::vector<uint8_t> data = {0x61, 0x6e, 0x6f};
    EXPECT_EQ(decode("616e6f"), data);

    EXPECT_THROW(decode("FFF", 3, data.data(), data.size()), basen::Exception);
    EXPECT_THROW(decode("!@#!", 4, data.data(), data.size()), basen::Exception);
    EXPECT_THROW(decode("FF", 2, data.data(), 0), basen::Exception);
    EXPECT_NO_THROW(decode("", 0, data.data(), 0));
}
TEST(hex, decode_1e7)
{
    std::string str(1e7, '0');
    decode(str);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}