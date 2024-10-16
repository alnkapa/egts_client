#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include "globals.h"
#include "queue.h"
#include <atomic>
#include <boost/asio.hpp>
#include <ctime>
#include <error/error.h>
#include <fstream>
#include <mutex>
#include <pub_sub.h>
#include <record/record.h>
#include <record/subrecord/firmware/firmware.h>
#include <record/subrecord/sr_command_data/sr_command_data.h>
#include <record/subrecord/sr_ext_pos_data/sr_ext_pos_data.h>
#include <record/subrecord/sr_pos_data/sr_pos_data.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <record/subrecord/sr_term_identity/sr_term_identity.h>
#include <record/subrecord/subrecord.h>
#include <transport/transport.h>
#include <unordered_map>

using boost::asio::ip::tcp;

inline MyQueue g_send_queue{};

void
my_read(tcp::socket &socket) noexcept;

void
my_read_file() noexcept;

inline std::atomic<bool> g_keep_running(true);

using G_PUB_RECORD_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SRRecordResponse>;
inline G_PUB_RECORD_TYPE g_pub_record{};

using G_PUB_RESULT_CODE_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SrResultCode>;
inline G_PUB_RESULT_CODE_TYPE g_pub_result_code{};

inline std::atomic<std::uint16_t> g_record_number(0);
inline std::atomic<std::uint16_t> g_packet_identifier(0);

inline egts::v1::buffer_type
make_new_packet(egts::v1::record::ServiceType source_service_type, egts::v1::buffer_type &&sub)
{
    return egts::v1::transport::Packet(
               g_packet_identifier++,
               egts::v1::record::wrapper(
                   g_record_number++,
                   source_service_type,
                   source_service_type,
                   std::move(sub)))
        .buffer();
}

inline std::ifstream g_nmea_file;

inline std::tm g_start_time{
    2010 - 1900, // tm_year
    0,           // tm_mon
    1,           // tm_mday
    0,           // tm_hour
    0,           // tm_min
    0,           // tm_sec
    0,           // tm_wday
    0,           // tm_yday
    0            // tm_isdst
}; // 00:00:00 01.01.2010 UTC

inline std::time_t g_start_time_t = std::mktime(&g_start_time);

class FileHolder
{
  private:
    std::mutex lo;
    struct value
    {
        uint16_t expected_parts_quantity{};
        egts::v1::record::subrecord::ObjectDataHeader odh;
        egts::v1::buffer_type data{};
    };
    std::unordered_map<uint16_t, value> m_value;

    void
    save_to_disc(value &&);

  public:
    void
    add_part(egts::v1::record::subrecord::SrPartData &&);
    void
    add_full(egts::v1::record::subrecord::SrFullData &&);
};

#endif // my_globals_H
