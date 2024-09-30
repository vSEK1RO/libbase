#include <basen/hash/sha256.hpp>
#include <basen/hex.hpp>
#include <gtest/gtest.h>

using namespace hash;

TEST(hash, sha256)
{
    EXPECT_EQ(hex::encode(sha256(hex::decode("1234"))), "3a103a4e5729ad68c02a678ae39accfbc0ae208096437401b7ceab63cca0622f");
}
TEST(hash, sha256_1e4)
{
    std::vector<uint8_t> data(32);
    for (size_t i = 0; i < 1e4; i++)
    {
        sha256(data);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}