#ifndef PUB_SUB_H
#define PUB_SUB_H
#include <algorithm>
#include <forward_list>
#include <functional>
#include <memory>
#include <mutex>

namespace pubsub
{

template <typename... T>
class Publisher
{
  public:
    struct Subscriber
    {
        virtual void
        callback(T...) = 0;
        virtual ~Subscriber() = default;
    };

    class Function : public Subscriber
    {
      private:
        std::function<void(T...)> m_fu;

      public:
        Function(std::function<void(T...)> fu)
            : m_fu(fu) {};
        void
        callback(T... args) override
        {
            m_fu(std::forward<T>(args)...);
        };
    };

  private:
    std::forward_list<std::weak_ptr<Subscriber>> subscribers;
    std::mutex m;

  public:
    virtual ~Publisher() = default;
    Publisher() = default;

    Publisher(const Publisher &) = delete;
    Publisher &
    operator=(const Publisher &) = delete;

    virtual void
    subscribe(std::weak_ptr<Subscriber> in)
    {
        std::lock_guard<std::mutex> lock(m);
        subscribers.emplace_front(in);
    }

    virtual void
    unsubscribe(std::weak_ptr<Subscriber> in)
    {
        std::lock_guard<std::mutex> lock(m);
        auto pred = subscribers.before_begin();
        auto current = subscribers.begin();
        auto in_locked = in.lock();
        while (current != subscribers.end())
        {
            if (auto v = current->lock())
            {
                if (v == in_locked)
                {
                    current = subscribers.erase_after(pred);
                }
                else
                {
                    pred = current;
                    ++current;
                }
            }
            else
            {
                current = subscribers.erase_after(pred);
            }
        }
    }

    virtual void
    notify(T... args)
    {
        std::lock_guard<std::mutex> lock(m);
        auto pred = subscribers.before_begin();
        auto current = subscribers.begin();
        while (current != subscribers.end())
        {
            if (auto l = current->lock())
            {
                (l->callback)(std::forward<T>(args)...);
                pred = current;
                ++current;
            }
            else
            {
                current = subscribers.erase_after(pred);
            }
        }
    }
};

} // namespace pubsub
#endif
