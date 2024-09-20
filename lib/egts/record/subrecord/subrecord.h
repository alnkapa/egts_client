#pragma once
#ifndef SUB_RECORD_H
#define SUB_RECORD_H

#include <cstdint> // uint8_t
#include <error.h>
#include <span>
#include <vector>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::record::frame_buffer_type;

using record_payload_type = egts::v1::record::record_payload_type;

inline bool
has_remaining_bytes(record_payload_type buffer, record_payload_type::iterator ptr, std::size_t x)
{
    if (ptr < buffer.begin() || ptr > buffer.end())
    {
        return false;
    }
    return std::distance(ptr, buffer.end()) >= x;
};

enum class Type
{
    // This sub-record is used to confirm the processing of
    // Service Level Support Protocol records.
    // This type of sub-record must be supported by all services.
    EGTS_SR_RECORD_RESPONSE = 0,

    /* ------------------------ EGTS_AUTH_SERVICE ----------------------------------------- */

    // Used only by ASN when requesting authorization from the authorizing TP
    // and contains the credentials of the ASN.
    EGTS_SR_TERM_IDENTITY = 1,

    // Intended for transmitting information about the infrastructure on the ASN side,
    // including the composition, state, and parameters of the ASN's blocks and modules.
    // This sub-record is optional, and the ASN developer decides
    // whether to fill in the fields and send this sub-record.
    // One sub-record describes one module.
    // Multiple such sub-records can be transmitted sequentially in one record,
    // allowing data about individual components of the entire hardware part
    // of the ASN and peripheral equipment to be sent.
    EGTS_SR_MODULE_DATA = 2,

    // This sub-record is used by the ASN to transmit information about the vehicle to the TP.
    EGTS_SR_VEHICLE_DATA = 3,

    // This sub-record is used only by the authorizing TP when requesting authorization
    // from the authorizing TP and contains the credentials of the authorized ASN.
    EGTS_SR_DISPATCHER_IDENTITY = 5,

    // This sub-record is used by the authorizing TP to transmit data to the ASN
    // about the method and parameters of encryption required for further interaction.
    EGTS_SR_AUTH_PARAMS = 6,

    // This sub-record is used by the authorizing TP to transmit data to the ASN
    // about the method and parameters of encryption required for further interaction.
    EGTS_SR_AUTH_INFO = 7,

    // This type of sub-record is used to inform the receiving party,
    // ASN or TP, depending on the direction of sending, about the supported services,
    // as well as to request a specific set of required services (from ASN to TP).
    EGTS_SR_SERVICE_INFO = 8,

    // This sub-record is used by the authorizing TP to inform the ASN
    // about the results of the ASN authentication procedure.
    EGTS_SR_RESULT_CODE = 9,

    /*------------------------- EGTS_TELEDATA_SERVICE ------------------------------------------*/

    // Used by ASN when transmitting primary location data.
    EGTS_SR_POS_DATA = 16,

    // Used by ASN when transmitting additional location data.
    EGTS_SR_EXT_POS_DATA = 17,

    // Used by ASN to transmit information about the state of additional discrete and analog inputs.
    EGTS_SR_AD_SENSORS_DATA = 18,

    // Used by the hardware-software complex to transmit data about counting inputs to the ASN.
    EGTS_SR_COUNTERS_DATA = 19,

    // Used to transmit information about the state of the ASN.
    EGTS_SR_STATE_DATA = 20,

    // Used by ASN to transmit acceleration data.
    EGTS_SR_ACCEL_DATA = 21,

    // Used by ASN to transmit data about the state of loop-in inputs.
    EGTS_SR_LOOPIN_DATA = 22,

    // Used by ASN to transmit data about the state of a discrete input.
    EGTS_SR_ABS_DIG_SENS_DATA = 23,

    // Used by ASN to transmit data about the state of an analog input.
    EGTS_SR_ABS_AN_SENS_DATA = 24,

    // Used by ASN to transmit data about the state of a counting input.
    EGTS_SR_ABS_CNTR_DATA = 25,

    // Used by ASN to transmit data about the state of a loop-in input.
    EGTS_SR_ABS_LOOPIN_DATA = 26,

    // Used by ASN to transmit data from liquid level sensors.
    EGTS_SR_LIQUID_LEVEL_SENSOR = 27,

    // Used by ASN to transmit data from passenger flow counters.
    EGTS_SR_PASSENGERS_COUNTERS = 28,

    /* ------------------------ EGTS_FIRMWARE_SERVICE ----------------------------------------- */

    // This sub-record is intended for transmitting data to the ASN,
    // which is divided into parts and transmitted sequentially. This sub-record is used for transmitting
    // large objects whose length does not allow them to be sent to the ASN in one packet.
    EGTS_SR_SERVICE_PART_DATA = 33,

    // This sub-record is intended for transmitting data to the ASN,
    // which is not divided into parts and is transmitted in one packet.
    EGTS_SR_SERVICE_FULL_DATA = 34,

    /* ------------------------ EGTS_COMMAND_SERVICE ----------------------------------------- */

    // This sub-record is used by ASN and TP to transmit commands, informational messages,
    // delivery confirmations, command execution confirmations, message read confirmations, etc.
    EGTS_SR_COMMAND_DATA = 51
};

// maximum size of the frame data
constexpr uint16_t max_frame_size{65495};

// The format of a separate entry in the Service Level Support Protocol.
class SubRecord
{
  private:
    // Type of entry.
    Type m_type{0};
    // Length of data in bytes for the entry in the SRD field.
    uint16_t m_length{0};
    // record payload
    record_payload_type mp_data;

  public:
    error::Error
    parse(record_payload_type, record_payload_type::iterator &) noexcept;

    Type
    type() const;

    record_payload_type
    data() const;
};

// make a subrecord buffer from a specific subrecord buffer.
frame_buffer_type
wrapper(Type type, frame_buffer_type &&data);

struct SubRec
{
    virtual ~SubRec() = default;
    virtual void parse(record_payload_type) = 0;
    virtual frame_buffer_type
    buffer() const noexcept = 0;
};

Record
make_package(std::initializer_list<frame_buffer_type> args);

} // namespace egts::v1::record::subrecord

#endif