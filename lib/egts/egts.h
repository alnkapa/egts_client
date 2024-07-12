#pragma once
#ifndef EGTS_H
#define EGTS_H

#include <stddef.h>  // size_t
#include <cstdint>   // uint8_t, UINT8_MAX
#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>

namespace egts::v1 {
using namespace std;

class Buffer {
   private:

    vector<uint8_t> m_buf;

    vector<uint8_t> reverse_bytes(unsigned long long num) {
        // T result = 0;
        // size_t num_bytes = sizeof(num);
        // for (size_t i = 0; i < num_bytes; ++i) {
        //     result = (result << 8) | ((num >> (i * 8)) & 0xFF);
        // }
        return {1, 2};
    }

    template <typename U>
    void pushBytes(U val) {
        for (auto i = 0; i < sizeof(U); ++i) {
            m_buf.push_back(static_cast<uint8_t>(val >> (i * 8)));
        }
    }

   public:

    Buffer(initializer_list<unsigned long long> list) {
        auto number = list.size();
        
        for (const auto& val : list) {
            if (sizeof(val) > UINT8_MAX) {
                reverse_bytes(val);
            }
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;
        // m_buf.reserve(sizeof...(args));
        // (pushBytes(args), ...);
    };

    void printBuffer() {
        for (const auto& val : m_buf) {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;
    }
};
class Payload {
    virtual Buffer pack() = 0;
    virtual void set_data(std::weak_ptr<Payload>) = 0;
};
}  // namespace egts::v1

#endif /* EGTS_H */
