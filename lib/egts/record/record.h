#pragma once
#ifndef RECORD_H
#define RECORD_H

#include "../error/error.h"
#include <cstdint> // uint8_t
#include <span>
#include <vector>

namespace egts::v1::record
{

using frame_buffer_type = std::vector<unsigned char>;

using record_payload_type = std::span<const unsigned char>;

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

    uint8_t m_source_service_type{};
    uint8_t m_recipient_service_type{};

    // record payload
    record_payload_type mp_data;

  public:
    error::Error
    parse(record_payload_type, record_payload_type::iterator &) noexcept;

    uint16_t
    record_number() const;

    record_payload_type
    data() const;

    uint8_t
    source_service_type() const;

    uint8_t
    recipient_service_type() const;
};

// make record buffer from subrecord buffer
frame_buffer_type
wrapper(
    uint16_t record_number,
    uint8_t source_service_type,
    uint8_t recipient_service_type,
    frame_buffer_type &&data);

} // namespace egts::v1::record

#endif