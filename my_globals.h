#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include "queue.h"
#include <boost/asio.hpp>
#include <error/error.h>
#include <iostream>
#include <pub_sub.h>
#include <record/record.h>
#include <record/subrecord/sr_record_response/sr_record_response.h>
#include <record/subrecord/sr_result_code/sr_result_code.h>
#include <record/subrecord/sr_term_identity/sr_term_identity.h>
#include <record/subrecord/subrecord.h>
#include <thread>
#include <transport/transport.h>
#include <utility>

using boost::asio::ip::tcp;

inline MyQueue g_send_queue{};



void
my_read(tcp::socket &socket) noexcept;

using G_PUB_RECORD_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SRRecordResponse>;
inline G_PUB_RECORD_TYPE g_pub_record{};

using G_PUB_RESULT_CODE_TYPE = pubsub::Publisher<egts::v1::record::subrecord::SrResultCode>;
inline G_PUB_RESULT_CODE_TYPE g_pub_result_code{};
// inline std::atomic<std::uint16_t> g_record_number(0);
// inline std::atomic<std::uint16_t> g_packet_identifier(0);

template <typename... Args>
egts::v1::transport::Packet
make_package(Args... args)
{
    // for (args) {
    //     для каждого парамера вызвать buffer()
    // }
    // // send sr_term_identity

    // record::subrecord::SrTermIdentity i{};
    // i.IMEI = {};
    // i.buffer_size = 512;

    // auto record_number = g_record_number++;

    // auto sub = record::subrecord::wrapper(
    //     record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
    //     i.buffer());

    // auto rec = record::wrapper(
    //     record_number,
    //     record::ServiceType::EGTS_AUTH_SERVICE,
    //     record::ServiceType::EGTS_AUTH_SERVICE,
    //     std::move(sub));

    // transport::Packet pkg{};
    // pkg.identifier(g_packet_identifier++);
    // pkg.set_frame(std::move(rec));

    return {};
}

template <typename... Args>
egts::v1::record::Record
make_record(Args... args)
{
    // for (args) {
    //     для каждого парамера вызвать buffer()
    // }
    // // send sr_term_identity

    // record::subrecord::SrTermIdentity i{};
    // i.IMEI = {};
    // i.buffer_size = 512;

    // auto record_number = g_record_number++;

    // auto sub = record::subrecord::wrapper(
    //     record::subrecord::Type::EGTS_SR_TERM_IDENTITY,
    //     i.buffer());

    // auto rec = record::wrapper(
    //     record_number,
    //     record::ServiceType::EGTS_AUTH_SERVICE,
    //     record::ServiceType::EGTS_AUTH_SERVICE,
    //     std::move(sub));

    // transport::Packet pkg{};
    // pkg.identifier(g_packet_identifier++);
    // pkg.set_frame(std::move(rec));

    return {};
}

#endif // my_globals_H
