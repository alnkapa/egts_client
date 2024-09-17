// #include "lib/egts/error/error.h"
// #include "lib/egts/transport/transport.h"
// #include "queue.h"
// #include <atomic>
// #include <boost/asio.hpp>
// #include <iostream>

// const int MAX_SEND_TRY = 3;
// // Because there is only one connection
// int g_unsuccess_send_attempts{0};

// class StateLessReaderWriter : public std::enable_shared_from_this<StateLessReaderWriter>
// {
//   private:
//     boost::asio::ip::tcp::socket m_socket;
//     boost::asio::io_context &m_io_context;
//     StateLessQueue &m_queue;
//     std::atomic<std::uint16_t> packet_number{0};

//     boost::system::error_code
//     do_write(const egts::v1::transport::frame_buffer_type &buffer)
//     {
//         boost::system::error_code ec;
//         boost::asio::write(m_socket, boost::asio::buffer(buffer), ec);
//         return ec;
//     }

//     void
//     async_do_write(egts::v1::transport::Packet &&pkg)
//     {
//         pkg.identifier(++packet_number);
//         auto buf = std::move(pkg.buffer());
//         // m_queue.wait_for_confirmed(
//         //     [self = shared_from_this()](egts::v1::transport::Packet &&pkg)
//         //     {
//         //         self->async_do_write(std::move(pkg));
//         //     });
//         boost::asio::async_write(
//             m_socket,
//             boost::asio::buffer(buf),
//             [self = shared_from_this()](
//                 const boost::system::error_code &ec,
//                 std::size_t bytes_transferred)
//             {
//                 if (!ec)
//                 {
//                     std::cerr << "write header size: " << bytes_transferred << std::endl;
//                     // self->wait_for_send();
//                 }
//                 else
//                 {
//                     ++g_unsuccess_send_attempts;
//                     std::cerr << "Error header: " << ec.message() << std::endl;
//                 }
//             });
//     }

//     void
//     async_do_read_header_to_buffer()
//     {
//         std::array<unsigned char, egts::v1::transport::header_length> header;
//         boost::asio::async_read(
//             m_socket,
//             boost::asio::buffer(header),
//             [self = shared_from_this(), &header](
//                 const boost::system::error_code &ec,
//                 std::size_t bytes_transferred)
//             {
//                 if (!ec)
//                 {
//                     std::cerr << "read header size: " << bytes_transferred << std::endl;
//                     auto pkg = std::make_unique<egts::v1::transport::Packet>();
//                     auto err = pkg->parse_header(header);
//                     if (err == egts::v1::error::Code::EGTS_PC_OK)
//                     {
//                         if (pkg->frame_data_length() > 0)
//                         {
//                             self->async_do_read_frame(std::move(pkg));
//                         }
//                         else
//                         {
//                             if (pkg->is_response())
//                             {
//                                 // тип пакета — это ответ, и фрейм пустой
//                                 // то это ошибка.
//                             }
//                             else
//                             {
//                                 auto rsp_pkg = pkg->make_response(err);
//                                 // self->m_queue.wait_for_push(std::move(rsp_pkg));
//                                 self->async_do_read_header_to_buffer();
//                             }
//                         }
//                     }
//                     else
//                     {
//                         auto rsp_pkg = pkg->make_response(err);
//                         rsp_pkg.identifier(++self->packet_number);
//                         self->do_write(rsp_pkg.buffer());
//                         std::cerr << "Error reading header: " << err.what() << std::endl;
//                     }
//                 }
//                 else
//                 {
//                     std::cerr << "Error reading header: " << ec.message() << std::endl;
//                 }
//             });
//     }

//     void
//     async_do_read_frame(std::unique_ptr<egts::v1::transport::Packet> pkg)
//     {
//         egts::v1::transport::frame_buffer_type frame(
//             pkg->frame_data_length() +
//                 egts::v1::transport::crc_data_length,
//             0);
//         boost::asio::async_read(
//             m_socket,
//             boost::asio::buffer(frame),
//             [self = shared_from_this(), pkg = std::move(pkg), &frame](
//                 const boost::system::error_code &ec,
//                 std::size_t bytes_transferred) mutable
//             {
//                 if (!ec)
//                 {
//                     std::cerr << "read frame size: " << bytes_transferred << std::endl;
//                     auto err = pkg->parse_frame(std::move(frame));
//                     if (err == egts::v1::error::Code::EGTS_PC_OK)
//                     {
//                         if (pkg->is_response())
//                         {
//                             auto resp = pkg->response();
//                             if (resp.second.OK())
//                             {
//                                 // self->m_queue.mark_as_confirmed(resp.first);
//                                 self->async_do_read_header_to_buffer();
//                             }
//                             else
//                             {
//                                 std::cerr << "Error reading frame: " << resp.second.what() << std::endl;
//                             }
//                         }
//                         else
//                         {
//                             auto rsp_pkg = pkg->make_response(err);
//                             // self->m_queue.wait_for_push(std::move(rsp_pkg));
//                             self->async_do_read_header_to_buffer();
//                         }
//                     }
//                     else
//                     {
//                         if (!pkg->is_response())
//                         {
//                             auto rsp_pkg = pkg->make_response(err);
//                             rsp_pkg.identifier(++self->packet_number);
//                             self->do_write(rsp_pkg.buffer());
//                         }
//                         std::cerr << "Error reading frame: " << err.what() << std::endl;
//                     }
//                 }
//                 else
//                 {
//                     std::cerr << "Error reading frame: " << ec.message() << std::endl;
//                 }
//             });
//     }

//   public:
//     StateLessReaderWriter(boost::asio::io_context &io_context, boost::asio::ip::tcp::socket socket, StateLessQueue &queue)
//         : m_io_context(io_context), m_socket(std::move(socket)), m_queue(queue)
//     {
//         m_queue.subscribe(
//             [self = shared_from_this()](egts::v1::transport::Packet &&pkg)
//             {
//                 self->async_do_write(std::move(pkg));
//             });
//     }

//     void
//     start_client()
//     {
//         async_do_read_header_to_buffer();
//     }
// };

// int
// main(int argc, char *argv[])
// {
//     boost::asio::io_context io_context{};
//     boost::asio::ip::tcp::resolver resolver(io_context);
//     boost::asio::ip::tcp::resolver::results_type endpoints;
//     boost::asio::ip::tcp::socket socket(io_context);
//     StateLessQueue queue(io_context);

//     while (g_unsuccess_send_attempts < MAX_SEND_TRY)
//     {
//         try
//         {
//             endpoints = resolver.resolve("alfa.shatl-t.ru", "34329");
//             boost::asio::connect(socket, endpoints);
//             std::make_shared<StateLessReaderWriter>(io_context, std::move(socket), queue)->start_client();
//             io_context.run();
//         }
//         catch (const std::exception &e)
//         {
//             // Смотреть на ошибку и, если необходимо, перезапустить клиента.
//             break;

//             std::cerr << "error: " << e.what() << std::endl;
//         };
//     }
//     return 0;
// }
