#include <boost/asio.hpp>
#include "lib/egts/transport/transport.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <tuple>
#include <unordered_set>

class Queue
{
  private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<egts::v1::transport::Packet> m_queue{};
    std::unordered_set<egts::v1::transport::Packet> m_store{};

  public:
    Queue(boost::asio::io_context &io_context);
    Queue(const Queue &) = delete; // mutex
    Queue &
    operator=(const Queue &) = delete; // mutex
    egts::v1::transport::Packet wait_for_send();
    void wait_for_confirmed(egts::v1::transport::Packet&&);
    void mark_as_confirmed(std::uint16_t);
    void wait_for_push(egts::v1::transport::Packet&&);
};
