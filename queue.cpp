#include "queue.h"

// StateLessQueue::StateLessQueue(boost::asio::io_context &io_context)
// {
// }

// egts::v1::transport::Packet
// StateLessQueue::wait_for_send()
// {
//     std::unique_lock<std::mutex> lock(m);
//     cv.wait(lock, [this]
//             { return !m_queue.empty(); });
//     auto item = m_queue.front();
//     m_queue.pop();
//     return item;
// }

// void
// StateLessQueue::wait_for_confirmed(egts::v1::transport::Packet &&item)
// {
//     std::lock_guard<std::mutex> lock(m);
//     m_store.emplace(std::move(item));
// }

// void
// StateLessQueue::mark_as_confirmed(std::uint16_t item)
// {
//     std::lock_guard<std::mutex> lock(m);
//     auto it = std::find_if(
//         m_store.begin(),
//         m_store.end(),
//         [item](const egts::v1::transport::Packet &packet)
//         {
//             return packet.identifier() == item;
//         });
//     if (it != m_store.end())
//     {
//         // элемент найден
//         m_store.erase(it);
//     }
//     else
//     {
//         // элемент не найден
//     }
//     // ..
// }

// void
// StateLessQueue::wait_for_push(egts::v1::transport::Packet &&item)
// {
//     std::lock_guard<std::mutex> lock(m);
//     m_queue.push(std::move(item));
//     cv.notify_one();
// }

void
StateLessQueue::process_queue(egts::v1::transport::Packet &&pkg)
{
    if (m_wait_for_send)
    {
        m_wait_for_send(std::move(pkg));
    }
}

StateLessQueue::StateLessQueue(boost::asio::io_context &io_context)
    : m_io_context(io_context)
{
}

void
StateLessQueue::subscribe(std::function<void(egts::v1::transport::Packet &&)> callback)
{
    m_wait_for_send = std::move(callback);
}

void
StateLessQueue::push(egts::v1::transport::Packet &&pkg)
{
    m_io_context.post(
        [this, pkg = std::move(pkg)]() mutable
        {
            process_queue(std::move(pkg));
        });
}
