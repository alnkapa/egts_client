#pragma once
#ifndef CRC_H
#define CRC_H

#include <cstdint>  // uint8_t, UINT8_MAX
#include <numeric>

namespace egts::v1 {

// count crc8
//
//	Name  : CRC-8
//	Poly  : 0x31    x^8 + x^5 + x^4 + 1
//	Init  : 0xFF
//	Revert: false
//	XorOut: 0x00
//	Check : 0xF7 ("123456789")
template <typename Iterator>
std::uint8_t crc8(Iterator begin, Iterator end, std::uint8_t crc = 0xFF) {
    for (auto it = begin; it != end; ++it) {
        crc ^= *it;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
};

// count crc16
//
// Name  : CRC-16 CCITT
// Poly  : 0x1021    x^16 + x^12 + x^5 + 1
// Init  : 0Xffff
// Revert: false
// XorOut: 0x0000
// Check : 0x29B1 (“123456789”)
template <typename Iterator>
std::uint16_t crc16(Iterator begin, Iterator end, std::uint16_t crc = 0xFFFF) {
    for (auto it = begin; it != end; ++it) {
        crc ^= static_cast<std::uint16_t>(*it) << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
};

}  // namespace egts::v1

#endif /* CRC_H */
