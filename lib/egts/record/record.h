#ifndef RECORD_H
#define RECORD_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>
#include <stddef.h> // size_t

namespace egts::v1::record
{

enum class ServiceType
{
    UNDEFINED = 0,
    // This service type is used for the authentication procedure
    // of the ASN (Authorized Terminal) on the Authorizing Terminal. When using the TCP/IP protocol as a transport,
    // the ASN (Authorized Terminal) must undergo this procedure, and only after successful completion of this procedure
    // does further interaction take place.
    EGTS_AUTH_SERVICE = 1,

    // This service is intended for processing telematics information
    // (coordinate data, sensor activation data, etc.) received from the ASN.
    // The service is described in Appendix B of this GOST.
    EGTS_TELEDATA_SERVICE = 2,

    // This service type is intended for processing control and configuration commands,
    // informational messages, and statuses transmitted between the ASN, TP, and operators.
    EGTS_COMMANDS_SERVICE = 4,

    // This service is intended for transmitting configuration and the actual software
    // (SW) of the hardware part of the ASN, as well as various peripheral equipment connected
    // to the ASN that supports the possibility of remote software updates.
    EGTS_FIRMWARE_SERVICE = 9,

    EGTS_ECALL_SERVICE = 10
};

using buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// maximum size of the frame data
constexpr uint16_t max_frame_size{65498};

// The structure of a separate record of the Service Level Agreement.
class Record
{
  private:
    // The parameter defines the size of the data from the RD field.
    uint16_t m_length{0};
    // Record number.
    //
    // The values in this field change according to the rules
    // of a cyclic counter in the range from 0 to 65535,
    // meaning that when the value reaches 65535,
    // the next value should be 0.
    // The value of this field is used for record confirmation.
    uint16_t m_number{0};

    ServiceType m_source_service_type{ServiceType::UNDEFINED};
    ServiceType m_recipient_service_type{ServiceType::UNDEFINED};

    // record payload
    payload_type mp_data;

  public:
    void
    parse(payload_type, payload_type::iterator &);

    uint16_t
    record_number() const;

    payload_type
    data() const;

    ServiceType
    source_service_type() const;

    ServiceType
    recipient_service_type() const;

    uint16_t
    length() const;
};

// make record buffer from subrecord buffer
buffer_type
wrapper(
    uint16_t record_number,
    ServiceType source_service_type,
    ServiceType recipient_service_type,
    buffer_type &&data);

} // namespace egts::v1::record

std::ostream &
operator<<(std::ostream &os, const egts::v1::record::ServiceType &serviceType);

#endif