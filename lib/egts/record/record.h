#pragma once
#ifndef RECORD_H
#define RECORD_H

#include <cstdint> // uint8_t
#include <vector>

namespace egts::v1::record
{

// maximum size of the frame data
constexpr uint16_t max_frame_size{65498};

// The structure of a separate record of the Service Level Agreement.
class Record
{
  private:
    // The parameter defines the size of the data from the RD field.
    uint16_t m_record_length{0};
    // Record number.
    //
    // The values in this field change according to the rules
    // of a cyclic counter in the range from 0 to 65535,
    // meaning that when the value reaches 65535,
    // the next value should be 0.
    // The value of this field is used for record confirmation.
    uint16_t m_record_number{0};

    // record payload
    std::vector<unsigned char> mp_data;

  public:
    Record(uint16_t record_number = 0);

    virtual ~Record();

    // parse
    error::Error
    parse(std::vector<unsigned char> &&buffer) noexcept;
};

} // namespace egts::v1::record

#endif