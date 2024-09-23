#include "error.h"
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <gtest/gtest.h>

using namespace egts::v1::record::subrecord;
using namespace egts::v1::error;

TEST(SR_RESULT_CODE_PARSE, BasicTests)
{
    SrResultCode rd{};
    rd.record_status=Code::EGTS_PC_ALREADY_EXISTS;
    auto buf = rd.buffer();

    SrResultCode rd1{};
    
    try
    {
        rd1.parse(buf);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }    
}


int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
