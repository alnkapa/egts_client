#include "queue.h"

queue_type
MyQueue::pop() noexcept
{    
    std::unique_lock<std::mutex> lock(m); // ! std::bad_alloc  -> 8 байт на стеке
    cv.wait(lock, [this]
            { return !m_queue.empty(); });

    auto pkg = std::move(m_queue.front());
    m_queue.pop();
    return pkg;
}

void MyQueue::push(queue_type &&pkg) noexcept
{
    std::lock_guard<std::mutex> lock(m); // ! std::bad_alloc -> 8 байт на стеке
    m_queue.push(std::move(pkg));
    cv.notify_one();
}
