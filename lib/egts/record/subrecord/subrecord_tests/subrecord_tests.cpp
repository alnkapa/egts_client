#include <algorithm> // Для std::fill
#include <error/error.h>
#include <gtest/gtest.h>
#include <record/subrecord/subrecord.h>

using namespace egts::v1::record;
using namespace egts::v1;

TEST(PARSE_SUBRECORD, BasicTests)
{
    subrecord::SubRecord srd{};
    subrecord::buffer_type data(2, 0);
    subrecord::payload_type s(data.cbegin(), data.cend());
    auto ptr = s.begin();
    try
    {
        srd.parse(s, ptr);
        FAIL() << "no exception";
    }
    catch (const egts::v1::error::Error &err)
    {
        if (err != error::Code::EGTS_PC_INVDATALEN)
        {
            ADD_FAILURE() << "error: " << err.what();
        }
    }

    data.resize(3 + subrecord::max_frame_size + 1);
    std::uint16_t length = subrecord::max_frame_size + 1;
    data[1] = static_cast<std::uint8_t>(length);      // Младший байт
    data[2] = static_cast<std::uint8_t>(length >> 8); // Старший байт
    egts::v1::payload_type s1(data.cbegin(), data.cend());
    ptr = s1.begin();
    try
    {
        srd.parse(s1, ptr);
        FAIL() << "no exception";
    }
    catch (const egts::v1::error::Error &err)
    {
        if (err != error::Code::EGTS_PC_INVDATALEN)
        {
            ADD_FAILURE() << "error: " << err.what();
        }
    }

    data.resize(3 + 10);
    length = 10;
    data[0] = static_cast<std::uint8_t>(subrecord::Type::EGTS_SR_ABS_CNTR_DATA);
    data[1] = static_cast<std::uint8_t>(length);      // Младший байт
    data[2] = static_cast<std::uint8_t>(length >> 8); // Старший байт
    std::fill(data.begin() + 3, data.end(), 44);
    egts::v1::payload_type s2(data.cbegin(), data.cend());
    ptr = s2.begin();
    try
    {
        srd.parse(s2, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    auto rez = srd.data();
    for (auto i = 0; i < 10; ++i)
    {
        if (data[i + 3] != rez[i])
        {
            ADD_FAILURE() << "error: " << data[i + 7] << " !=  " << rez[i];
        };
    }
}

TEST(PARSE1_SUBRECORD, BasicTests)
{

    auto rez = subrecord::wrapper(subrecord::Type::EGTS_SR_ABS_CNTR_DATA, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    subrecord::SubRecord srd{};
    subrecord::payload_type s(rez.cbegin(), rez.cend());
    auto ptr = s.begin();
    try
    {
        srd.parse(s, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }
    if (srd.type() != subrecord::Type::EGTS_SR_ABS_CNTR_DATA)
    {
        ADD_FAILURE() << "error: " << static_cast<std::uint8_t>(srd.type()) << " != " << static_cast<std::uint8_t>(subrecord::Type::EGTS_SR_ABS_CNTR_DATA);
    }

    auto rez1 = srd.data();
    auto data = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (auto i = 0; i < 10; ++i)
    {
        if (data[i] != rez1[i])
        {
            ADD_FAILURE() << "error: " << data[i] << " !=  " << rez1[i];
        };
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
