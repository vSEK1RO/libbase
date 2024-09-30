#include <basen/Exception.hpp>
#include <gtest/gtest.h>

TEST(Exception, Exception)
{
    EXPECT_ANY_THROW(throw basen::Exception(basen::Exception::Code::BASE));
}
TEST(Exception, message)
{
    try
    {
        throw basen::Exception(basen::Exception::Code::BASE);
    }
    catch (const basen::Exception &e)
    {
        EXPECT_STREQ(e.message(), "incorrect base");
    }
}
TEST(Exception, code)
{
    try
    {
        throw basen::Exception(basen::Exception::Code::BASE);
    }
    catch (const basen::Exception &e)
    {
        EXPECT_EQ(uint32_t(e.code()), uint32_t(basen::Exception::Code::BASE));
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}