#include "error.h"
#include "sr_result_code.h"
#include <algorithm> // Для std::fill
#include <gtest/gtest.h>

using namespace egts::v1::record::subrecord;
using namespace egts::v1::error;

TEST(PARSE, BasicTests)
{
    SrResultCode rd{};
    rd.record_status=Code::EGTS_PC_ALREADY_EXISTS;
    auto buf = rd.buffer();

    SrResultCode rd1{};
    auto err = rd1.parse(buf);
    if (err != Code::EGTS_PC_OK)
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
