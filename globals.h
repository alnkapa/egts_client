// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "queue.h"
// #include <atomic>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// inline std::atomic<bool> g_running(true);
inline MyPacketQueue g_send_queue{};

void
my_read(tcp::socket &socket) noexcept;

#endif // GLOBALS_H
