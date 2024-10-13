#include "../pub_sub.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class MySubscriber : public pubsub::Publisher<int>::Subscriber
{
  public:
    MySubscriber(int id)
        : id(id), receivedCount(0) {}

    void
    callback(int value) override
    {
        std::cout << "Subscriber " << id << " received: " << value << std::endl;

        receivedCount++;
    }

    int
    getReceivedCount() const
    {
        return receivedCount;
    }

  private:
    int id;
    int receivedCount;
};

void
publisherThread(pubsub::Publisher<int> &publisher)
{
    for (int i = 0; i < 10; ++i)
    {
        publisher.notify(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void
subscriberThread(pubsub::Publisher<int> &publisher, int id)
{
    auto subscriber = std::make_shared<MySubscriber>(id);
    publisher.subscribe(subscriber);

    // Подписчик будет получать уведомления в течение 5 секунд
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Отписываемся
    publisher.unsubscribe(subscriber);
}

void
concurrentTest(pubsub::Publisher<int> &publisher)
{
    // Запускаем поток для публикации
    std::thread pubThread(publisherThread, std::ref(publisher));

    // Запускаем несколько потоков для подписки
    const int numSubscribers = 5;
    std::vector<std::thread> subThreads;
    for (int i = 0; i < numSubscribers; ++i)
    {
        subThreads.emplace_back(subscriberThread, std::ref(publisher), i + 1);
    }

    // Ждем завершения всех потоков
    pubThread.join();
    for (auto &t : subThreads)
    {
        t.join();
    }
}

int
main()
{
    pubsub::Publisher<int> publisher;

    // Запускаем тест
    concurrentTest(publisher);

    std::cout << "All threads have finished." << std::endl;

    return 0;
}
