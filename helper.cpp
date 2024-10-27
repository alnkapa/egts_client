#include "my_globals.h"

// VisitorHolder::VisitorHolder(tcp::socket &socket)
//     : m_socket(socket)
// {
// }

// void
// VisitorHolder::operator()(egts::v1::transport::Packet &&pkg)
// {
//     pkg.identifier(g_packet_identifier++);
//     if (boost::asio::write(
//             m_socket,
//             boost::asio::buffer(pkg.buffer()),
//             boost::asio::transfer_all()) != pkg.buffer().size())
//     {
//         throw std::runtime_error("send size error");
//     };
// }

// void
// VisitorHolder::operator()(egts::v1::record::subrecord::SRRecordResponse &&response)
// {
//     // TODO: для загрузки файлов, будет нужно потом
// }

// void
// VisitorHolder::operator()(egts::v1::record::subrecord::SrResultCode &&resultCode)
// {
//     auto err = resultCode.error();
//     if (!err.OK())
//     {
//         std::cerr << "auth: error: " << err.what() << std::endl;
//         throw std::runtime_error("auth: error");
//     }
//     else
//     {
//         std::cout << "auth: ok" << std::endl;

//         if (g_nmea_file.is_open())
//         {
//             std::thread reader(my_read_file); // to exit g_keep_running = false
//             reader.detach();
//         }
//     }
// }

// void
// VisitorHolder::operator()(egts::v1::record::subrecord::SrCommandData &&commandData)
// {
//     // TODO: что то сделать по командам
//     std::cout << "CMD code: " << commandData.data.code << std::endl;
//     std::cout << "CMD data: " << commandData.data.data() << std::endl;
//     if (commandData.ct_com())
//     {
//         // command response
//         commandData.command_type = egts::v1::record::subrecord::CommandType::CT_COMCONF;

//         auto sub = egts::v1::record::subrecord::wrapper(
//             egts::v1::record::subrecord::Type::EGTS_SR_COMMAND_DATA,
//             commandData.buffer());

//         auto buffer = make_new_packet(
//             egts::v1::record::ServiceType::EGTS_COMMANDS_SERVICE,
//             std::move(sub));

//         if (boost::asio::write(
//                 m_socket,
//                 boost::asio::buffer(buffer),
//                 boost::asio::transfer_all()) != buffer.size())
//         {
//             throw std::runtime_error("send size error");
//         }
//     }
// }

// void
// VisitorHolder::operator()(egts::v1::record::subrecord::SrPartData &&partData)
// {
//     m_file_holder.add_part(std::move(partData));
// }

// void
// VisitorHolder::operator()(egts::v1::record::subrecord::SrFullData &&fullData)
// {
//     m_file_holder.add_full(std::move(fullData));
// }

// void
// VisitorHolder::operator()(Done)
// {
//     throw egts::v1::error::Error{};
// }
