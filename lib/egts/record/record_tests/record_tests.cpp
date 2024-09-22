#include "error.h"
#include "record.h"
#include <algorithm> // Для std::fill
#include <gtest/gtest.h>

using namespace egts::v1;

TEST(RECORD_PARSE, BasicTests)
{
    record::Record rd{};
    record::buffer_type data(2, 0);

    try
    {
        record::payload_type s(data.cbegin(), data.cend());
        auto ptr = s.begin();
        rd.parse(s, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        if (err != error::Code::EGTS_PC_INVDATALEN)
        {
            ADD_FAILURE() << "error: " << err.what();
        }
    }
    try
    {
        data.resize(7 + record::max_frame_size + 1);
        std::uint16_t length = record::max_frame_size + 1;
        data[0] = static_cast<std::uint8_t>(length);      // Младший байт
        data[1] = static_cast<std::uint8_t>(length >> 8); // Старший байт
        record::payload_type s1(data.cbegin(), data.cend());
        auto ptr = s1.begin();
        rd.parse(s1, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        if (err != error::Code::EGTS_PC_INVDATALEN)
        {
            ADD_FAILURE() << "error: " << err.what();
        }
    }
    try
    {
        data.resize(7 + 10);
        std::uint16_t length = 10;
        data[0] = static_cast<std::uint8_t>(length);      // Младший байт
        data[1] = static_cast<std::uint8_t>(length >> 8); // Старший байт
        data[2] = static_cast<std::uint8_t>(33);          // Младший байт
        data[3] = static_cast<std::uint8_t>(33 >> 8);     // Старший байт
        data[4] = static_cast<std::uint8_t>(0);           // flag
        data[5] = static_cast<std::uint8_t>(1);           // SST
        data[6] = static_cast<std::uint8_t>(2);           // RST
        std::fill(data.begin() + 7, data.end(), 44);

        record::payload_type s2(data.cbegin(), data.cend());
        auto ptr = s2.begin();
        rd.parse(s2, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    auto rez = rd.data();
    for (auto i = 0; i < 10; ++i)
    {
        if (data[i + 7] != rez[i])
        {
            ADD_FAILURE() << "error: " << data[i + 7] << " !=  " << rez[i];
        };
    }
}

TEST(RECORD_PARSE1, BasicTests)
{

    auto rez = record::wrapper(10, record::ServiceType::EGTS_AUTH_SERVICE, record::ServiceType::EGTS_COMMANDS_SERVICE, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    record::Record rd{};
    try
    {
        record::payload_type s(rez.cbegin(), rez.cend());
        auto ptr = s.begin();
        rd.parse(s, ptr);
    }
    catch (const egts::v1::error::Error &err)
    {
        ADD_FAILURE() << "error: " << err.what();
    }

    if (rd.record_number() != 10)
    {
        ADD_FAILURE() << "error: " << rd.record_number() << " != " << 10;
    }

    if (rd.source_service_type() != record::ServiceType::EGTS_AUTH_SERVICE)
    {
        ADD_FAILURE() << "error: " << static_cast<std::uint8_t>(rd.source_service_type()) << " != " << static_cast<std::uint8_t>(record::ServiceType::EGTS_AUTH_SERVICE);
    }

    if (rd.recipient_service_type() != record::ServiceType::EGTS_COMMANDS_SERVICE)
    {
        ADD_FAILURE() << "error: " << static_cast<std::uint8_t>(rd.recipient_service_type()) << " != " << static_cast<std::uint8_t>(record::ServiceType::EGTS_COMMANDS_SERVICE);
    }

    auto rez1 = rd.data();
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
