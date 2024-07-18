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

uint8_t crc8(uint8_t init_value, uint8_t value) {

};

uint16_t crc16(uint16_t init_value, uint16_t value) {

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
