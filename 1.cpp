#include <boost/system/error_code.hpp>
#include <iostream>

int main() {
    for (int i = 0; i <= 125; ++i) { // Примерный диапазон кодов ошибок
        boost::system::error_code ec(i, boost::system::generic_category());
        std::cout << "Code: " << ec.value() << ", Message: " << ec.message() << std::endl;
    }
    return 0;
}

/*
void async_do_write()
{
    // Вместо блокировки, используем post для обработки очереди
    boost::asio::post(m_socket.get_executor(), [self = shared_from_this()]() {
        auto pkg = std::move(self->m_queue.wait_for_send());
        pkg.identifier(++self->packet_number);
        auto buf = std::move(pkg.buffer());
        self->m_queue.wait_for_confirmed(std::move(pkg));
        
        boost::asio::async_write(
            self->m_socket,
            boost::asio::buffer(buf),
            [self](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                if (!ec)
                {
                    std::cerr << "write header size: " << bytes_transferred << std::endl;
                    self->async_do_write();
                }
                else
                {
                    ++g_unsuccess_send_attempts;
                    std::cerr << "Error header: " << ec.message() << std::endl;
                }
            });
    });
}

#include <boost/asio.hpp>
#include "lib/egts/transport/transport.h"
#include <queue>
#include <unordered_set>

class Queue
{
private:
    std::queue<egts::v1::transport::Packet> m_queue{};
    std::unordered_set<egts::v1::transport::Packet> m_store{};
    boost::asio::io_context &m_io_context;

public:
    Queue(boost::asio::io_context &io_context) : m_io_context(io_context) {}

    Queue(const Queue &) = delete; // mutex
    Queue &operator=(const Queue &) = delete; // mutex

    void wait_for_push(egts::v1::transport::Packet &&item)
    {
        m_queue.push(std::move(item));
        // Запускаем обработку очереди
        boost::asio::post(m_io_context, [this]() { process_queue(); });
    }

    egts::v1::transport::Packet wait_for_send()
    {
        // Здесь можно использовать асинхронный механизм для ожидания
        // Например, можно использовать таймер или другой подход
        // В этом примере просто возвращаем пакет, если он есть
        if (!m_queue.empty())
        {
            auto item = std::move(m_queue.front());
            m_queue.pop();
            return item;
        }
        // Если очередь пуста, можно вернуть пустой пакет или выбросить исключение
        throw std::runtime_error("Queue is empty");
    }

    void wait_for_confirmed(egts::v1::transport::Packet &&item)
    {
        m_store.emplace(std::move(item));
    }

    void mark_as_confirmed(std::uint16_t item)
    {
        auto it = std::find_if(
            m_store.begin(),
            m_store.end(),
            [item](const egts::v1::transport::Packet &packet)
            {
                return packet.identifier() == item;
            });
        if (it != m_store.end())
        {
            m_store.erase(it);
        }
    }

private:
    void process_queue()
    {
        // Обработка очереди, например, отправка пакетов
        while (!m_queue.empty())
        {
            auto pkg = wait_for_send();
            // Здесь можно отправить пакет или выполнить другую логику
        }
    }
};


*/