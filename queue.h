// #include "lib/egts/subrecord/sr_record_response/sr_record_response.h"
// #include "lib/egts/subrecord/sr_result_code/sr_result_code.h"
#include "lib/egts/transport/transport.h"
#include <condition_variable>
#include <mutex>
#include <queue>

using namespace egts::v1;

class MyPacketQueue
{
  private:
    std::mutex m;
    std::queue<transport::Packet> m_queue;
    std::condition_variable cv;

  public:
    MyPacketQueue() = default;

    MyPacketQueue(const MyPacketQueue &) = delete; // !mutex
    MyPacketQueue &
    operator=(const MyPacketQueue &) = delete; // !mutex

    transport::Packet
    pop() noexcept;

    void
    push(transport::Packet &&) noexcept;
};
