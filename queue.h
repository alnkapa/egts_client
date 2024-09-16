#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <future>

// #include <condition_variable>
// #include <mutex>
// #include <queue>
// #include <tuple>
// #include <unordered_set>
// std::queue<egts::v1::transport::Packet> m_queue{};
// std::unordered_set<egts::v1::transport::Packet> m_store{};

class StateLessQueue
{
  private:
    boost::asio::io_context &m_io_context;
    std::function<void(egts::v1::transport::Packet &&)> m_wait_for_send;
    void
    process_queue(egts::v1::transport::Packet &&pkg);

  public:
    StateLessQueue(boost::asio::io_context &io_context);
    void subscribe(std::function<void(egts::v1::transport::Packet &&)>);
    // StateLessQueue(const StateLessQueue &) = delete; // mutex
    // StateLessQueue &
    // operator=(const StateLessQueue &) = delete; // mutex
    // egts::v1::transport::Packet wait_for_send();
    // void wait_for_confirmed(std::function<void(egts::v1::transport::Packet &&)>);
    // void mark_as_confirmed(std::uint16_t);
    void
    push(egts::v1::transport::Packet &&);
};
