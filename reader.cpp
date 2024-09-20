#include "globals.h"
#include "lib/egts/error/error.h"
#include "lib/egts/record/record.h"
#include "lib/egts/record/subrecord/sr_record_response/sr_record_response.h"
#include "lib/egts/record/subrecord/sr_result_code/sr_result_code.h"
#include "lib/egts/record/subrecord/sr_term_identity/sr_term_identity.h"
#include "lib/egts/record/subrecord/subrecord.h"
#include "lib/egts/transport/transport.h"
#include <iostream>

void
my_read(tcp::socket &socket) noexcept
{
    using namespace egts::v1;

    transport::header_buffer_type header_buffer{};
    boost::system::error_code ec;
    error::Error err;
    // read header
    boost::asio::read(socket, boost::asio::buffer(header_buffer), boost::asio::transfer_all(), ec);
    if (ec)
    {
        // TODO: обработать ошибку
        std::cerr << "transport: read: error: " << ec.message() << std::endl;
        return;
    }
    transport::Packet pkg{};
    err = pkg.parse_header(header_buffer);
    if (err != error::Code::EGTS_PC_OK)
    {
        // TODO: обработать ошибку
        std::cerr << "transport: read: error: " << err.what() << std::endl;
        return;
    }
    if (pkg.frame_data_length() > 0)
    {
        // read payload
        transport::frame_buffer_type frame_buffer(pkg.frame_data_length() + transport::crc_data_length, 0);
        boost::asio::read(socket, boost::asio::buffer(frame_buffer), boost::asio::transfer_all(), ec);
        if (ec)
        {
            // TODO: обработать ошибку
            std::cerr << "transport: read: error: " << ec.message() << std::endl;
            return;
        }
        err = pkg.parse_frame(std::move(frame_buffer));
        if (err != error::Code::EGTS_PC_OK)
        {
            // TODO: обработать ошибку
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            return;
        }

        std::cout << "transport: read: size: " << transport::header_length + pkg.frame_data_length() + transport::crc_data_length << std::endl;
        // record level
        auto buffer = pkg.get_frame();
        auto ptr = buffer.begin();
        while (ptr != buffer.end())
        {
            record::Record rec{};
            auto err = rec.parse(buffer, ptr);
            if (err != error::Code::EGTS_PC_OK)
            {
                // TODO: обработать ошибку
                std::cerr << "transport: record: error: " << err.what() << std::endl;
                return;
            }
            using namespace egts::v1::record;
            // subrecord level
            auto sub_buffer = rec.data();
            auto sub_ptr = buffer.begin();
            while (sub_ptr != sub_buffer.end())
            {
                subrecord::SubRecord s_rec{};
                err = s_rec.parse(sub_buffer, sub_ptr);
                if (err != error::Code::EGTS_PC_OK)
                {
                    // TODO: обработать ошибку
                    std::cerr << "transport: subrecord: error: " << err.what() << std::endl;
                    return;
                }
                try
                {
                    // Only these subrecords may come from the server.
                    switch (s_rec.type())
                    {
                    case subrecord::Type::EGTS_SR_RECORD_RESPONSE:
                    {
                        auto sub_rec_resp = std::make_unique<subrecord::SRRecordResponse>();
                        sub_rec_resp->parse(s_rec.data());
                        //g_sub_record_response.notify(sub_rec_resp.get());
                    }
                    break;
                    case subrecord::Type::EGTS_SR_RESULT_CODE:
                    {
                        // subrecord::SrResultCode sub_rec_code;
                        // err = sub_rec_code.parse(s_rec.data());
                        // if (err != error::Code::EGTS_PC_OK)
                        // {
                        //     // TODO: обработать ошибку
                        //     std::cerr << "transport: subrecord: error: " << err.what() << std::endl;
                        //     return;
                        // }
                        // g_sub_result_code.notify(std::move(sub_rec_code));
                    }
                    break;
                    case subrecord::Type::EGTS_SR_SERVICE_PART_DATA:
                        // TODO
                        break;
                    case subrecord::Type::EGTS_SR_SERVICE_FULL_DATA:
                        // TODO
                        break;
                    case subrecord::Type::EGTS_SR_COMMAND_DATA:
                        // TODO
                        break;
                    default:
                        // The rest is simply ignored.
                        break;
                    }
                }
                catch (const error::Error &err)
                {
                    // TODO: обработать ошибку
                    std::cerr << "transport: subrecord: error: " << err.what() << std::endl;
                    return;
                }
                catch (...)
                {
                    // TODO: обработать ошибку
                    std::cerr << "transport: subrecord: error: " << err.what() << std::endl;
                    return;
                }
            }
        }
    }
}
