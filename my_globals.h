#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include "queue.h"
#include <atomic>
#include <boost/asio.hpp>
#include <error/error.h>
#include <fstream>
#include <memory>
#include <ctime>
#include <pub_sub.h>
#include <record/record.h>
#include <record/subrecord/sr_pos_data/sr_pos_data.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <record/subrecord/sr_term_identity/sr_term_identity.h>
#include <record/subrecord/subrecord.h>
#include <transport/transport.h>

using boost::asio::ip::tcp;

inline MyQueue g_send_queue{};

void
my_read(tcp::socket &socket) noexcept;

void
    my_read_file(std::shared_ptr<std::ifstream>) noexcept;

inline std::atomic<bool> g_keep_running(true);

using G_PUB_RECORD_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SRRecordResponse>;
inline G_PUB_RECORD_TYPE g_pub_record{};

using G_PUB_RESULT_CODE_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SrResultCode>;
inline G_PUB_RESULT_CODE_TYPE g_pub_result_code{};

inline std::atomic<std::uint16_t> g_record_number(0);
inline std::atomic<std::uint16_t> g_packet_identifier(0);

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
#endif // my_globals_H