#include "error.h"
#include "transport.h"
#include <algorithm> // Для std::fill
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <vector>

using namespace egts::v1::transport;
using Error = egts::v1::error::Error;
using Code = egts::v1::error::Code;

TEST(PARSE_HEADER, BasicTests)
{
    Packet pr{};
    std::array<std::uint8_t, header_length> data1 = {'1', '0', '0', '0', '0', '0', '0', '0', '0'};

    auto er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_UNS_PROTOCOL)
    {
        ADD_FAILURE() << "protocol_version test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 1 << 6;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_UNS_PROTOCOL)
    {
        ADD_FAILURE() << "PRF test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 1 << 5;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_ROUTE_DENIED)
    {
        ADD_FAILURE() << "rte test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 1 << 4;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_DECRYPT_ERROR)
    {
        ADD_FAILURE() << "ENA test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 1 << 2;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_INC_DATAFORM)
    {
        ADD_FAILURE() << "CMP test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 0;
    data1[3] = 33;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_INC_HEADERFORM)
    {
        ADD_FAILURE() << "HL test failed: error is: " << er.what();
    }
    data1[0] = protocol_version;
    data1[2] = 0;
    data1[3] = header_length;
    data1[9] = 11;
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_UNS_TYPE)
    {
        ADD_FAILURE() << "packet_type test failed: error is: " << er.what();
    }

    data1[0] = protocol_version;
    data1[2] = 0;
    data1[3] = header_length;
    std::fill(data1.begin() + 4, data1.end() - 1, 2);
    data1[9] = static_cast<std::uint8_t>(Type::EGTS_PT_APPDATA);
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_HEADERCRC_ERROR)
    {
        ADD_FAILURE() << "CRC test failed: error is: " << er.what();
    }

    data1[header_length - 1] = egts::v1::crc8(data1.begin(), data1.end() - crc_header_length);
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_OK)
    {
        ADD_FAILURE() << "CRC test failed: error is: " << er.what();
    }

    std::uint16_t frame_data_length = max_frame_size;
    data1[5] = static_cast<std::uint8_t>(frame_data_length);      // Младший байт
    data1[6] = static_cast<std::uint8_t>(frame_data_length >> 8); // Старший байт
    data1[header_length - 1] = egts::v1::crc8(data1.begin(), data1.end() - crc_header_length);
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_OK)
    {
        ADD_FAILURE() << "frame_data_length test failed: error is: " << er.what();
    }
    if (pr.frame_data_length() != frame_data_length)
    {
        ADD_FAILURE() << "frame_data_length1 test failed ";
    }

    std::uint16_t packet_identifier = 0xFFFF;
    data1[7] = static_cast<std::uint8_t>(packet_identifier);      // Младший байт
    data1[8] = static_cast<std::uint8_t>(packet_identifier >> 8); // Старший байт
    data1[header_length - 1] = egts::v1::crc8(data1.begin(), data1.end() - crc_header_length);
    er = pr.parse_header(data1);
    if (er != Code::EGTS_PC_OK)
    {
        ADD_FAILURE() << "packet_identifier test failed: error is: " << er.what();
    }
    if (pr.packet_identifier() != packet_identifier)
    {
        ADD_FAILURE() << "packet_identifier test failed ";
    }
}

TEST(PARSE_FRAME, BasicTests)
{
    Packet pr{};
    std::vector<std::uint8_t> frame = {'1', '1', '1', '1', '1', '1', '1', '1', '1'};
    std::uint16_t frame_data_length = frame.size();
    std::array<std::uint8_t, header_length> header = {
        protocol_version,                                  // 0  PRV
        0,                                                 // 1  SKID
        0,                                                 // 2  FLAG
        header_length,                                     // 3  HeaderLength
        0,                                                 // 4  HeaderEncoding
        static_cast<std::uint8_t>(frame_data_length),      // 5  FrameDataLength 0
        static_cast<std::uint8_t>(frame_data_length >> 8), // 6  FrameDataLength 1
        0,                                                 // 7  PacketIdentifier 0
        0,                                                 // 8  PacketIdentifier 1
        0,                                                 // 9  PacketType
        0,                                                 // 10 HeaderCheckSum
    };
    header[header_length - 1] = egts::v1::crc8(header.begin(), header.end() - crc_header_length);
    auto er = pr.parse_header(header);
    if (er != Code::EGTS_PC_OK)
    {
        ADD_FAILURE() << "packet test failed: error is: " << er.what();
    }

    std::vector<std::uint8_t> frame_copy(frame_data_length, 0);
    std::copy(frame.begin(), frame.end(), frame_copy.begin());
    er = pr.parse_frame(std::move(frame_copy));
    if (er != Code::EGTS_PC_INVDATALEN)
    {
        ADD_FAILURE() << "frame1 test failed: error is: " << er.what();
    }

    frame_copy.resize(frame_data_length + crc_data_length, 0);
    er = pr.parse_frame(std::move(frame_copy));
    if (er != Code::EGTS_PC_DATACRC_ERROR)
    {
        ADD_FAILURE() << "crc test failed: error is: " << er.what();
    }

    uint16_t crc16_val = egts::v1::crc16(frame.begin(), frame.end());
    frame_copy.resize(frame_data_length, 0);
    std::copy(frame.begin(), frame.end(), frame_copy.begin());
    frame_copy.emplace_back(static_cast<std::uint8_t>(crc16_val));
    frame_copy.emplace_back(static_cast<std::uint8_t>(crc16_val >> 8));
    er = pr.parse_frame(std::move(frame_copy));
    if (er != Code::EGTS_PC_OK)
    {
        ADD_FAILURE() << "frame2 test failed: error is: " << er.what();
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}