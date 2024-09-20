#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H

#include <error/error.h>
#include <transport/transport.h>
#include <record/record.h>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void
my_read(tcp::socket &socket) noexcept;

//#include "lib/egts/error/error.h"
//#include "lib/egts/record/record.h"
// #include "lib/egts/record/subrecord/sr_record_response/sr_record_response.h"
// #include "lib/egts/record/subrecord/sr_result_code/sr_result_code.h"
// #include "lib/egts/record/subrecord/sr_term_identity/sr_term_identity.h"
// #include "lib/egts/record/subrecord/subrecord.h"
//#include "lib/egts/transport/transport.h"

// #include "lib/egts/error/error.h"
// #include "lib/egts/record/record.h"
// #include "lib/egts/subrecord/sr_record_response/sr_record_response.h"
// #include "lib/egts/subrecord/sr_result_code/sr_result_code.h"
// #include "lib/egts/subrecord/sr_term_identity/sr_term_identity.h"
// #include "lib/egts/subrecord/subrecord.h"
// #include "lib/egts/transport/transport.h"
// #include "lib/pub_sub/pub_sub.h"
// #include "queue.h"
// #include <boost/asio.hpp>


// inline std::atomic<std::uint16_t> g_record_number(0);
// inline std::atomic<std::uint16_t> g_packet_identifier(0);
// inline MyPacketQueue g_send_queue{};
// // using sub_record_response_t = pubsub::Publisher<egts::v1::record::subrecord::SubRec*>;
// // inline sub_record_response_t g_sub_record_response{};


// template<typename ...Args>
// egts::v1::transport::Packet make_package(Args ...args)
// {
//     // for (args) {
//     //     для каждого парамера вызвать buffer()
//     // }
//     // // send sr_term_identity

//     // record::subrecord::SrTermIdentity i{};
//     // i.IMEI = {};
//     // i.buffer_size = 512;
    

//     // auto record_number = g_record_number++;

//     // auto sub = record::subrecord::wrapper(
//     //     record::subrecord::Type::EGTS_SR_TERM_IDENTITY, 
//     //     i.buffer());

//     // auto rec = record::wrapper(
//     //     record_number,
//     //     record::ServiceType::EGTS_AUTH_SERVICE,
//     //     record::ServiceType::EGTS_AUTH_SERVICE,
//     //     std::move(sub));

//     // transport::Packet pkg{};
//     // pkg.identifier(g_packet_identifier++);
//     // pkg.set_frame(std::move(rec));

//     return pkg;
// }

#endif // my_globals_H
