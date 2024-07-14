#pragma once
#ifndef EGTS_H
#define EGTS_H

#include <stddef.h>   // size_t
#include <algorithm>  // std::fill_n, std::copy, std::back_inserter
#include <cstdint>    // uint8_t, UINT8_MAX
#include <iostream>
#include <memory>       //weak_ptr
#include <type_traits>  // is_unsigned
#include <vector>       // vector

namespace egts::v1 {
using namespace std;

class Buffer;
template <typename T>
concept ValidType = is_unsigned_v<T> || is_same_v<T, Buffer>;

class Buffer {
   private:

    vector<uint8_t> m_buf;

    template <typename U>
        requires(is_unsigned_v<U>)
    void push_back(U val) {
        const auto size = sizeof(U);
        if (size > 1) {  // write in reversed order
            m_buf.resize(m_buf.size() + size);
            auto it = m_buf.end();
            for (auto i = 0; i < size; ++i) {
                --it;
                *(it) = static_cast<uint8_t>(val >> (i * 8));
            }
        } else {
            m_buf.emplace_back(static_cast<uint8_t>(val));
        }
    }

    void push_back(const Buffer& other) {
        // TODO:  move semantic ?
        const auto size = other.m_buf.size();
        if (size) {
            m_buf.resize(m_buf.size() + size);
            copy(other.m_buf.begin(), other.m_buf.end(),
                 std::back_inserter(m_buf));
        }
    }

   public:

    Buffer() = default;
    ~Buffer() = default;

    template <typename... Args>
        requires(ValidType<Args> && ...)
    Buffer(const Args&... args) {
        if constexpr (sizeof...(Args)) {
            auto size = (sizeof(args) + ...);
            m_buf.reserve(size);
            (push_back(args), ...);
        }
    };
    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    Buffer(const Buffer&& other){
        
    };

    Buffer& operator=(const Buffer&& other){

    };
    uint16_t size() {
        // TODO: check size if less than max_uint16
        return m_buf.size();
    }

    void check_sum(){

    };

    void printBuffer() {
        for (const auto& val : m_buf) {
            std::cout << static_cast<unsigned long long>(val) << " ";
        }
        std::cout << std::endl;
    }
};

class Payload {
   public:

    virtual Buffer pack() = 0;
    virtual void set_data(weak_ptr<Payload>) = 0;
};
}  // namespace egts::v1

#endif /* EGTS_H */
