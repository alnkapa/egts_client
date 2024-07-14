#pragma once
#ifndef CRC_H
#define CRC_H

#include <stddef.h>   // size_t
#include <algorithm>  // std::fill_n, std::copy, std::back_inserter
#include <cstdint>    // uint8_t, UINT8_MAX
#include <iostream>
#include <memory>       //weak_ptr
#include <type_traits>  // is_unsigned
#include <vector>       // vector

namespace egts::v1 {
using namespace std;

template <typename T>
class CRC {
   private:

    T m_int_value{};

   public:

    CRC(T int_value) : m_int_value(int_value){};
    CRC() = delete;
    template <typename InputIt>
    T operator()(InputIt begin, InputIt end) {
        T ret{m_int_value};
        for (auto it = begin; it != end; ++it) {
            ret += *it;
        };
        return ret;
    };
    template <std::ranges::input_range Range>
    T operator()(const Range& range) {
        return this(range.begin(), range.end());
    };
};

// class CRC8 : public CRC {
//    public:

//     // virtual Buffer pack() = 0;
//     // virtual void set_data(weak_ptr<Payload>) = 0;
// };

// class CRC16 : public CRC {
//    public:

//     // virtual Buffer pack() = 0;
//     // virtual void set_data(weak_ptr<Payload>) = 0;
// };

}  // namespace egts::v1

#endif /* CRC_H */
