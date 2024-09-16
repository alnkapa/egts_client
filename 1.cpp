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



#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <future>
#include <queue>
#include <unordered_set>

namespace egts {
namespace v1 {
namespace transport {
    struct Packet {
        // Define your Packet structure here
    };
}
}
}

class StateLessQueue {
private:
    boost::asio::io_context &m_io_context;
    std::queue<egts::v1::transport::Packet> m_queue; // Example queue for packets

public:
    StateLessQueue(boost::asio::io_context &io_context) : m_io_context(io_context) {}

    egts::v1::transport::Packet wait_for_send() {
        // Create a promise and future to wait for the completion
        std::promise<egts::v1::transport::Packet> promise;
        std::future<egts::v1::transport::Packet> future = promise.get_future();

        // Post the task to the io_context
        m_io_context.post([this, promise = std::move(promise)]() mutable {
            // Simulate sending a packet (you would replace this with your actual logic)
            if (!m_queue.empty()) {
                egts::v1::transport::Packet packet = m_queue.front();
                m_queue.pop();

                // Set the promise value to indicate completion
                promise.set_value(std::move(packet));
            } else {
                // Handle the case where the queue is empty
                // You might want to set an exception or a default packet
                promise.set_exception(std::make_exception_ptr(std::runtime_error("Queue is empty")));
            }
        });

        // Wait for the future to be ready and return the packet
        return future.get(); // This will block until the packet is ready
    }

    // Example method to push packets into the queue
    void push_packet(const egts::v1::transport::Packet &packet) {
        m_queue.push(packet);
    }
};

// Example usage
int main() {
    boost::asio::io_context io_context;
    StateLessQueue queue(io_context);

    // Push a packet into the queue
    queue.push_packet(egts::v1::transport::Packet{});

    // Call wait_for_send and get the packet
    try {
        egts::v1::transport::Packet packet = queue.wait_for_send();
        std::cout << "Packet sent!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <queue>
#include <unordered_set>

namespace egts {
namespace v1 {
namespace transport {
    struct Packet {
        // Определите структуру Packet здесь
    };
}
}
}

class StateLessQueue {
private:
    boost::asio::io_context &m_io_context;
    std::queue<egts::v1::transport::Packet> m_queue;

public:
    StateLessQueue(boost::asio::io_context &io_context) : m_io_context(io_context) {}

    void wait_for_send(std::function<void(std::optional<egts::v1::transport::Packet>)> callback) {
        m_io_context.post([this, callback]() {
            if (!m_queue.empty()) {
                egts::v1::transport::Packet packet = m_queue.front();
                m_queue.pop();
                callback(std::move(packet));
            } else {
                callback(std::nullopt); // Указываем, что значение отсутствует
            }
        });
    }

    void push_packet(const egts::v1::transport::Packet &packet) {
        m_queue.push(packet);
    }
};

// Пример использования
int main() {
    boost::asio::io_context io_context;
    StateLessQueue queue(io_context);

    // Добавляем пакет в очередь
    queue.push_packet(egts::v1::transport::Packet{});

    // Вызываем wait_for_send с коллбеком
    queue.wait_for_send([](std::optional<egts::v1::transport::Packet> packet) {
        if (packet) {
            std::cout << "Packet sent!" << std::endl;
        } else {
            std::cout << "No packet available!" << std::endl;
        }
    });

    // Запускаем io_context для обработки задач
    io_context.run();

    return 0;
}

#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <future>
#include <iostream>

class StateLessQueue {
private:
    boost::asio::io_context &m_io_context;

public:
    StateLessQueue(boost::asio::io_context &io_context) : m_io_context(io_context) {}

    // Метод для отправки пакета
    std::future<egts::v1::transport::Packet> push(egts::v1::transport::Packet&& packet) {
        // Используем std::async для асинхронной обработки пакета
        return std::async(std::launch::async, [this, packet = std::move(packet)]() mutable {
            // Здесь вы можете добавить логику обработки пакета
            std::cout << "Processing packet with ID: " << packet.id << std::endl;

            // Возвращаем обработанный пакет
            return packet;
        });
    }
};

// Пример использования
int main() {
    boost::asio::io_context io_context;
    StateLessQueue queue(io_context);

    // Создаем пакет
    egts::v1::transport::Packet packet{1}; // Пример пакета с id = 1

    // Отправляем пакет через метод push
    std::future<egts::v1::transport::Packet> future = queue.push(std::move(packet));

    // Ожидаем завершения обработки и получаем результат
    egts::v1::transport::Packet processed_packet = future.get();
    std::cout << "Packet sent with ID: " << processed_packet.id << std::endl;

    return 0;
}


*/

// #include "lib/egts/transport/transport.h"
// #include <boost/asio.hpp>
// #include <future>
// #include <iostream>

// class StateLessQueue {
// private:
//     boost::asio::io_context &m_io_context;
//     void m_wait_for_send(egts::v1::transport::Packet);

// public:
//     // Конструктор
//     StateLessQueue(boost::asio::io_context &io_context)
//         : m_io_context(io_context) {}

//     // Метод для отправки пакета
//     void push(egts::v1::transport::Packet &&packet) {
//         // Используем std::async для асинхронной обработки пакета
//         std::async(std::launch::async, [this, packet = std::move(packet)]() mutable {            
//             m_wait_for_send(packet);            
//         });
//     }    
// };

/*
#include "lib/egts/transport/transport.h"
#include <boost/asio.hpp>
#include <iostream>
#include <functional>

class StateLessQueue {
private:
    boost::asio::io_context &m_io_context;
    std::function<void(egts::v1::transport::Packet&&)> m_wait_for_send;

public:
    // Конструктор
    StateLessQueue(boost::asio::io_context &io_context)
        : m_io_context(io_context), m_wait_for_send(nullptr) {}

    // Метод для подписки на обработку пакетов
    void subscribe(std::function<void(egts::v1::transport::Packet&&)> callback) {
        m_wait_for_send = std::move(callback);
    }

    // Метод для отправки пакета
    void push(egts::v1::transport::Packet &&packet) {
        // Проверяем, подписан ли обработчик
        if (m_wait_for_send) {
            // Используем std::async для асинхронной обработки пакета
            std::async(std::launch::async, [this, packet = std::move(packet)]() mutable {
                // Вызываем подписанную функцию
                m_wait_for_send(std::move(packet));
            });
        } else {
            std::cerr << "No handler subscribed for packet processing." << std::endl;
        }
    }
};

// Пример функции, которая будет передана в StateLessQueue
void example_wait_for_send(egts::v1::transport::Packet packet) {
    std::cout << "Packet sent with ID: " << packet.id << std::endl;
}

int main() {
    boost::asio::io_context io_context;

    // Создаем экземпляр StateLessQueue
    StateLessQueue queue(io_context);

    // Подписываемся на обработку пакетов
    queue.subscribe(example_wait_for_send);

    // Создаем пакет
    egts::v1::transport::Packet packet{1}; // Пример пакета с id = 1

    // Отправляем пакет через метод push
    queue.push(std::move(packet));

    // Запускаем io_context для обработки задач
    io_context.run();

    return 0;
}

*/