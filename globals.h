// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <atomic>
#include "queue.h" 
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
extern std::atomic<bool> g_running;
extern MyPacketQueue g_send_queue;

void my_read(tcp::socket &socket) noexcept;

#endif // GLOBALS_H
