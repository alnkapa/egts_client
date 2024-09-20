#include "error.h"
#include "sr_record_response.h"
#include <algorithm> // Для std::fill
#include <gtest/gtest.h>

using namespace egts::v1::record::subrecord;
using namespace egts::v1::error;

TEST(PARSE, BasicTests)
{
    SRRecordResponse rd{};
    rd.confirmed_record_number = 10;
    rd.record_status = Code::EGTS_PC_ALREADY_EXISTS;
    auto buf = rd.buffer();

    SRRecordResponse rd1{};
    try
    {
        rd1.parse(buf);
    }
    catch (const Error &err)
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
