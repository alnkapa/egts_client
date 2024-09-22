#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include "queue.h"
#include <atomic>
#include <boost/asio.hpp>
#include <error/error.h>
#include <iostream>
#include <pub_sub.h>
#include <fstream>
#include <memory>
#include <record/record.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <record/subrecord/sr_term_identity/sr_term_identity.h>
#include <record/subrecord/subrecord.h>
#include <thread>
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

#endif // my_globals_H
