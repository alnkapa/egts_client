#include "my_globals.h"
#include "queue.h"
#include "record.h"
#include "record/subrecord/firmware/firmware.h"
#include "record/subrecord/sr_command_data/sr_command_data.h"
#include "record/subrecord/sr_record_response/sr_record_response.h"
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// subrecord level
void
my_sub_record_level(egts::v1::payload_type sub_buffer)
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
            g_send_queue.push(egts::v1::record::subrecord::SRRecordResponse(s_rec.data()));
            break;
        case Type::EGTS_SR_RESULT_CODE:
            g_send_queue.push(egts::v1::record::subrecord::SrResultCode(s_rec.data()));
            break;
        case Type::EGTS_SR_SERVICE_PART_DATA:
            g_send_queue.push(egts::v1::record::subrecord::SrPartData(s_rec.data()));
            break;
        case Type::EGTS_SR_SERVICE_FULL_DATA:
            g_send_queue.push(egts::v1::record::subrecord::SrFullData(s_rec.data()));
            break;
        case Type::EGTS_SR_COMMAND_DATA:
            g_send_queue.push(egts::v1::record::subrecord::SrCommandData(s_rec.data()));
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
my_record_level(egts::v1::payload_type buffer)
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

// create records for confirming received records.
egts::v1::buffer_type
my_make_record(std::span<std::uint32_t> in)
{
    using vec_t = std::vector<egts::v1::record::subrecord::SRRecordResponse>;

    std::unordered_map<
        egts::v1::record::ServiceType,
        vec_t>
        response_sub_records_by_service_type{};

    // Here we reviewed the incoming records for the services.
    for (const auto &v : in)
    {
        auto current_service = static_cast<egts::v1::record::ServiceType>((v & 0xFF00) >> 8);
        auto record_number = static_cast<std::uint16_t>((v & 0x00FF));

        auto sub_rec_res = egts::v1::record::subrecord::SRRecordResponse(
            record_number,
            {});

        auto it = response_sub_records_by_service_type.find(current_service);
        if (it != response_sub_records_by_service_type.end())
        {
            it->second.emplace_back(std::move(sub_rec_res));
        }
        else
        {
            response_sub_records_by_service_type.emplace(
                current_service,
                vec_t{sub_rec_res});
        }
    }

    // Records are created for each service, and corresponding subrecords are added to each record
    egts::v1::buffer_type record_buf;
    for (auto &[k, vec] : response_sub_records_by_service_type)
    {

        egts::v1::buffer_type buf;
        for (auto &v : vec)
        {
            buf += egts::v1::record::subrecord::wrapper(
                egts::v1::record::subrecord::Type::EGTS_SR_RECORD_RESPONSE,
                v.buffer());
        }

        auto record_number = g_record_number++;
        record_buf += egts::v1::record::wrapper(
            record_number,
            k,
            k,
            std::move(buf));
    }
    return record_buf;
}

void
my_read(tcp::socket &socket) noexcept
{
    egts::v1::transport::header_buffer_type header_buffer{};
    std::vector<std::uint32_t> received_number_records{};
    while (true)
    {
        egts::v1::transport::Packet pkg{};
        try
        {
            // read and parse header
            if (boost::asio::read(
                    socket,
                    boost::asio::buffer(header_buffer),
                    boost::asio::transfer_all()) != header_buffer.size())
            {
                throw std::runtime_error("header read size error");
            }
            pkg.parse_header(header_buffer);

            // if (frame) read and parse frame
            if (pkg.frame_data_length() > 0)
            {
                std::size_t size = pkg.frame_data_length() + egts::v1::transport::crc_data_length;
                egts::v1::transport::buffer_type frame_buffer(size, 0);
                if (boost::asio::read(
                        socket,
                        boost::asio::buffer(frame_buffer),
                        boost::asio::transfer_all()) != size)
                {
                    throw std::runtime_error("frame read size error");
                }
                pkg.parse_frame(std::move(frame_buffer));
                received_number_records = my_record_level(pkg.get_frame());
            }

            // response
            egts::v1::buffer_type rec_buffer;
            if (!received_number_records.empty()) // add the numbers of the sent records to the response
            {
                rec_buffer = my_make_record(received_number_records);
                received_number_records.clear();
            }
            if (!pkg.is_response())
            {
                auto resp_pkg = pkg.make_response({});
                if (!rec_buffer.empty())
                {
                    resp_pkg.set_frame(std::move(rec_buffer));
                }
                g_send_queue.push(std::move(resp_pkg));
            }
            else if (!rec_buffer.empty())
            {
                g_send_queue.push(egts::v1::transport::Packet(std::move(rec_buffer)));
            }
        }
        catch (const egts::v1::error::Error &err) // Protocol errors
        {
            if (!pkg.is_response())
            {
                g_send_queue.push(pkg.make_response(err));
            }
            std::cerr << "error: " << err.what() << "\n";
            break;
        }
        catch (const boost::system::error_code &err) // Connection errors
        {
            std::cerr << "error: " << err.message() << "\n";
            break;
        }
        catch (const std::exception &err) // Other errors
        {
            std::cerr << "error: " << err.what() << "\n";
            break;
        }
    }
    g_send_queue.push(Done{});
}
