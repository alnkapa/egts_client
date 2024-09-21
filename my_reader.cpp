#include "my_globals.h"
#include "queue.h"
#include <cstdint>
#include <vector>

// subrecord level
void
my_sub_record_level(egts::v1::record_payload_type sub_buffer)
{
    using egts::v1::record::subrecord::Type;
    auto sub_ptr = sub_buffer.begin();
    while (sub_ptr != sub_buffer.end())
    {
        egts::v1::record::subrecord::SubRecord s_rec{};
        s_rec.parse(sub_buffer, sub_ptr);
        // Only these subrecords may come from the server.
        switch (s_rec.type())
        {
        case Type::EGTS_SR_RECORD_RESPONSE:
        {
            egts::v1::record::subrecord::SRRecordResponse rsp;
            rsp.parse(s_rec.data());
            g_send_queue.push(std::move(rsp));
        }
        break;
        case Type::EGTS_SR_RESULT_CODE:
        {
            egts::v1::record::subrecord::SrResultCode rsp;
            rsp.parse(s_rec.data());
            g_send_queue.push(std::move(rsp));
        }
        break;
        case Type::EGTS_SR_SERVICE_PART_DATA:
            // TODO
            break;
        case Type::EGTS_SR_SERVICE_FULL_DATA:
            // TODO
            break;
        case Type::EGTS_SR_COMMAND_DATA:
            // TODO
            break;
        default:
            // The rest is simply ignored.
            break;
        }
    }
}

// record level
//
// It will return the numbers of the received (service<<8|record)=uint32.
std::vector<std::uint32_t>
my_record_level(egts::v1::record_payload_type buffer)
{
    auto ptr = buffer.begin();
    std::vector<std::uint32_t> rez{};
    rez.reserve(20);
    while (ptr != buffer.end())
    {
        egts::v1::record::Record rec{};
        rec.parse(buffer, ptr);
        rez.emplace_back(
            ((static_cast<std::uint32_t>(rec.source_service_type()) << 8) & 0xFF00) // service
            |
            (static_cast<std::uint32_t>(rec.record_number()) & 0x00FF) // record
        );
        my_sub_record_level(rec.data());
    }
    return rez;
}

egts::v1::frame_buffer_type
my_make_record_records(std::span<std::uint32_t>)
{
    return {};
}

// transport level
void
my_read(tcp::socket &socket) noexcept
{
    egts::v1::transport::header_buffer_type header_buffer{};
    std::vector<std::uint32_t> received_records{};
    while (true)
    {
        egts::v1::transport::Packet pkg{};
        try
        {
            boost::asio::read(
                socket,
                boost::asio::buffer(header_buffer),
                boost::asio::transfer_all());

            std::cout << "read: " << std::endl;
            for (auto v : header_buffer)
            {
                std::cout << std::hex << static_cast<int>(v);
            }
            std::cout << std::endl;
            pkg.parse_header(header_buffer);

            if (pkg.frame_data_length() > 0)
            {
                egts::v1::transport::frame_buffer_type frame_buffer(
                    pkg.frame_data_length() + egts::v1::transport::crc_data_length,
                    0);

                boost::asio::read(
                    socket,
                    boost::asio::buffer(frame_buffer),
                    boost::asio::transfer_all());

                pkg.parse_frame(std::move(frame_buffer));
                received_records = my_record_level(pkg.get_frame());
            }

            if (!pkg.is_response())
            {
                auto resp_pkg = pkg.make_response({});
                if (!received_records.empty())
                {
                    auto rec_buffer = my_make_record_records(received_records);
                    resp_pkg.set_frame(std::move(rec_buffer));
                    received_records.clear();
                }
                g_send_queue.push(std::move(resp_pkg));
            }
            else if (!received_records.empty())
            {
                egts::v1::transport::Packet new_pkg{};
                auto rec_buffer = my_make_record_records(received_records);
                new_pkg.set_frame(std::move(rec_buffer));
                received_records.clear();
                g_send_queue.push(std::move(new_pkg));
            }
        }
        catch (const egts::v1::error::Error &err) // Protocol errors
        {
            if (!pkg.is_response())
            {
                auto resp_pkg = pkg.make_response(err);
                g_send_queue.push(std::move(resp_pkg));
            }
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
        catch (const boost::system::error_code &err) // Connection errors
        {
            // определить  ошибку соединение, и есть надо завершить цикл
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
        catch (const std::exception &err) // Other errors
        {
            // завершить цикл
            std::cerr << "transport: read: error: " << err.what() << std::endl;
            break;
        }
    }
    g_send_queue.push(Done{});
}
