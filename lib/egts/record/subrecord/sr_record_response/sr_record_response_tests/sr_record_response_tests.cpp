#include "error.h"
#include <gtest/gtest.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>

TEST(SR_RECORD_RESPONSE_PARSE, BasicTests)
{
    egts::v1::record::subrecord::SRRecordResponse rd{};
    rd.confirmed_record_number = 10;
    rd.record_status = egts::v1::error::Code::EGTS_PC_ALREADY_EXISTS;
    auto buf = rd.buffer();

    egts::v1::record::subrecord::SRRecordResponse rd1{};
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
