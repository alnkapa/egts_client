#include "error.h"
#include "record/subrecord/sr_term_identity/sr_term_identity.h"
#include <gtest/gtest.h>
#include <string>

TEST(SR_TERM_IDENTITY_PARSE, BasicTests)
{
    egts::v1::record::subrecord::SrTermIdentity rd{};
    rd.IMEI = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    rd.buffer_size = 2;
    auto buf = rd.buffer();

    egts::v1::record::subrecord::SrTermIdentity rd1{};

    try
    {
        rd1.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    if (rd.IMEI != rd1.IMEI)
    {
        ADD_FAILURE() << "error: " << std::string(rd.IMEI.data()) << " != " << std::string(rd1.IMEI.data());
    }

    if (rd.buffer_size != rd1.buffer_size)
    {
        ADD_FAILURE() << "error: " << rd.buffer_size << " != " << rd1.buffer_size;
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
