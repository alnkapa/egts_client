// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "queue.h"
// #include <atomic>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

inline std::atomic<std::uint16_t> g_record_number(0);
inline std::atomic<std::uint16_t> g_packet_identifier(0);
inline MyPacketQueue g_send_queue{};

void
my_read(tcp::socket &socket) noexcept;

void
auth(tcp::socket &socket);

#endif // GLOBALS_H
