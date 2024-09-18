#include <utility>

#include <base/base58.hpp>
#include <base/hex.hpp>
#include <gtest/gtest.h>

using namespace base58;

std::pair<std::string, std::string> test = {
    "003812e515df45235c6bcc2233ac9d0c4ebbd781de",
    "167VUagc755PbQoB7cCTfTPjbQ5Nk6fEuD",
};

TEST(base58, encodeCheck)
{
    EXPECT_EQ(encodeCheck(hex::decode(test.first)), test.second);
}
TEST(base58, decodeCheck)
{
    EXPECT_EQ(test.first, hex::encode(decodeCheck(test.second)));
    try
    {
        decodeCheck("incorrect");
    }
    catch (const std::exception &e)
    {
        EXPECT_STREQ(e.what(), "base58::decodeCheck: checksum incorrect");
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}