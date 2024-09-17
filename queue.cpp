#include "queue.h"

transport::Packet
MyPacketQueue::pop() noexcept
{
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]
            { return !m_queue.empty(); });

    auto pkg = std::move(m_queue.front());
    m_queue.pop();
    return pkg;
}

void MyPacketQueue::push(transport::Packet &&pkg) noexcept
{
    std::lock_guard<std::mutex> lock(m);
    m_queue.push(std::move(pkg));
    cv.notify_one();
}
