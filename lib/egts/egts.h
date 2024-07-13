#pragma once
#ifndef EGTS_H
#define EGTS_H

#include <stddef.h>   // size_t
#include <algorithm>  // std::fill_n
#include <cstdint>    // uint8_t, UINT8_MAX
#include <iostream>
#include <memory>       //weak_ptr
#include <type_traits>  // is_unsigned
#include <vector>       // vector

namespace egts::v1 {
using namespace std;

class Buffer {
   private:

    vector<uint8_t> m_buf;

    template <typename U>
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

   public:

    template <typename... Args>
    requires(is_unsigned_v<Args>&&...) Buffer(Args... args) {
        auto size = (sizeof(args) + ...);
        m_buf.reserve(size);
        (push_back(args), ...);
    };

    void printBuffer() {
        for (const auto& val : m_buf) {
            std::cout << static_cast<unsigned long long>(val) << " ";
        }
        std::cout << std::endl;
    }
};
class Payload {
    virtual Buffer pack() = 0;
    virtual void set_data(weak_ptr<Payload>) = 0;
};
}  // namespace egts::v1

#endif /* EGTS_H */
