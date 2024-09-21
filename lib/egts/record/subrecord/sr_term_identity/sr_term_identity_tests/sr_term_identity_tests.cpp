#include "error.h"
#include <string>
#include "sr_term_identity.h"
#include <algorithm> // Для std::fill
#include <gtest/gtest.h>

using namespace egts::v1::record::subrecord;
using namespace egts::v1::error;

TEST(PARSE, BasicTests)
{
    // SrTermIdentity rd{};
    // rd.IMEI = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    // rd.buffer_size = 2;
    // auto buf = rd.buffer();

    // SrTermIdentity rd1{};
    // auto err = rd1.parse(buf);
    // if (err != Code::EGTS_PC_OK)
    // {
    //     ADD_FAILURE() << "error: " << err.what();
    // }

    // if (rd.IMEI != rd1.IMEI)
    // {
    //     ADD_FAILURE() << "error: " << std::string(rd.IMEI.data()) << " != " << std::string(rd1.IMEI.data());
    // }

    // if (rd.buffer_size != rd1.buffer_size)
    // {
    //     ADD_FAILURE() << "error: " << rd.buffer_size << " != " << rd1.buffer_size;
    // }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
