#pragma once
#ifndef EGTS_H
#define EGTS_H

#include <algorithm> // std::fill_n, std::copy, std::back_inserter
#include <cstdint>   // uint8_t, UINT8_MAX
#include <iostream>
#include <iterator>    // input_iterator_tag
#include <memory>      // weak_ptr
#include <stddef.h>    // size_t
#include <stdexcept>   // std::out_of_range
#include <type_traits> // is_unsigned
#include <vector>      // vector

namespace egts::v1
{

using namespace std;

class Buffer;

template <typename T, typename = void>
struct is_valid_type : std::false_type
{
};

template <typename T>
struct is_valid_type<T, std::enable_if_t<std::is_unsigned_v<T> || std::is_same_v<T, Buffer>>> : std::true_type
{
};

class Buffer
{
  private:
    vector<uint8_t> m_buf;

  public:
    Buffer() = default;
    ~Buffer() = default;

    template <typename U>
    void
    push_back(U val)
    {
        static_assert(is_valid_type<U>::value, "Invalid type for push_back");
        const auto size = sizeof(U);
        if (size > 1)
        { // write in reversed order
            m_buf.resize(m_buf.size() + size);
            auto it = m_buf.end();
            for (auto i = 0; i < size; ++i)
            {
                --it;
                *(it) = static_cast<uint8_t>(val >> (i * 8));
            }
        }
        else
        {
            m_buf.emplace_back(static_cast<uint8_t>(val));
        }
    }

    void
    push_back(const Buffer &other)
    {
        const auto size = other.m_buf.size();
        if (size)
        {
            m_buf.reserve(m_buf.size() + size);
            std::copy(other.m_buf.begin(), other.m_buf.end(), std::back_inserter(m_buf));
        }
    }

    template <typename... Args>
    Buffer(const Args &...args)
    {
        if constexpr (sizeof...(Args))
        {
            auto size = (sizeof(args) + ...);
            m_buf.reserve(size);
            (push_back(args), ...);
        }
    }

    uint16_t
    size() const
    {
        if (m_buf.size() > UINT16_MAX)
        {
            throw std::overflow_error("Buffer size exceeds UINT16_MAX");
        }
        return static_cast<uint16_t>(m_buf.size());
    }

    Buffer
    read(size_t length)
    {
        if (length > m_buf.size())
        {
            throw std::out_of_range("Requested length exceeds buffer size");
        }
        Buffer result;
        result.m_buf.insert(result.m_buf.end(), m_buf.begin(), m_buf.begin() + length);
        m_buf.erase(m_buf.begin(), m_buf.begin() + length); // Удаляем прочитанные байты из оригинального буфера
        return result;
    }

    uint8_t
    operator[](size_t index) const
    {
        if (index >= m_buf.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return m_buf[index];
    }

    bool
    empty() const
    {
        return m_buf.empty();
    }

    void
    printBuffer() const
    {
        for (const auto &val : m_buf)
        {
            std::cout << static_cast<unsigned long long>(val) << " ";
        }
        std::cout << std::endl;
    }

    class Iterator
    {
      private:
        vector<uint8_t>::iterator m_it;

      public:
        using iterator_category = input_iterator_tag;
        using value_type = uint8_t;
        using difference_type = std::ptrdiff_t;
        using pointer = uint8_t *;
        using reference = uint8_t &;

        explicit Iterator(vector<uint8_t>::iterator it)
            : m_it(it) {}
        Iterator &
        operator++()
        {
            ++m_it;
            return *this;
        }
        Iterator
        operator++(int)
        {
            Iterator retval = *this;
            ++(*this);
            return retval;
        }
        bool
        operator==(const Iterator &other) const
        {
            return m_it == other.m_it;
        }
        bool
        operator!=(const Iterator &other) const
        {
            return !(*this == other);
        }
        reference
        operator*() const
        {
            return *m_it;
        }
    };

    Iterator
    begin()
    {
        return Iterator{m_buf.begin()};
    }

    Iterator
    end()
    {
        return Iterator{m_buf.end()};
    }
};

class Payload
{
  public:
    virtual Buffer pack() = 0;
    virtual void set_data(weak_ptr<Payload>) = 0;
};

} // namespace egts::v1

#endif /* EGTS_H */
