#include "globals.h"
#include "lib/egts/error/error.h"
#include "lib/egts/record/record.h"
// #include "lib/egts/subrecord/sr_record_response/sr_record_response.h"
// #include "lib/egts/subrecord/sr_result_code/sr_result_code.h"
// #include "lib/egts/subrecord/sr_term_identity/sr_term_identity.h"
// #include "lib/egts/subrecord/subrecord.h"
#include "lib/egts/transport/transport.h"
#include <initializer_list>
#include <iostream>

// template<typename ...Args>
// egts::v1::transport::Packet make_package(Args ...args)
// {
//     // send sr_term_identity

//     record::subrecord::SrTermIdentity i{};
//     i.IMEI = {};
//     i.buffer_size = 512;
    

//     auto record_number = g_record_number++;

//     auto sub = record::subrecord::wrapper(record::subrecord::Type::EGTS_SR_TERM_IDENTITY, i.buffer());

//     auto rec = record::wrapper(
//         record_number,
//         record::ServiceType::EGTS_AUTH_SERVICE,
//         record::ServiceType::EGTS_AUTH_SERVICE,
//         std::move(sub));

//     transport::Packet pkg{};
//     pkg.identifier(g_packet_identifier++);
//     pkg.set_frame(std::move(rec));

//     return pkg;
// }
