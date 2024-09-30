#pragma once
#include <cstddef>
#ifndef SR_COMMAND_DATA_H
#define SR_COMMAND_DATA_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint32_t = std::uint32_t;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

enum class DataAct : uint8_t
{
    NO_ACT = 0,  // No action
    ASK_ACT = 1, // Request value
    SET_ACT = 2, // Set value
    ADD_ACT = 3, // Add new parameter
    DEL_ACT = 4  // Delete existing parameter
};

enum class CharSet : uint8_t
{
    CP1251 = 0,        // CP-1251
    IA5_ASCII = 1,     // IA5 (CCITT T.50)/ASCII (ANSI X3.4)
    BINARY_DATA_1 = 2, // Binary data
    LATIN_1 = 3,       // Latin 1 (ISO-8859-1)
    BINARY_DATA_2 = 4, // Binary data
    JIS = 5,           // JIS (X 0208-1990)
    CYRILLIC = 6,      // Cyrillic (ISO-8859-5)
    LATIN_HEBREW = 7,  // Latin/Hebrew (ISO-8859-8)
    UCS2 = 8           // UCS2 (ISO/IEC-10646)
};

enum class ConfirmationType : uint8_t
{
    CC_OK = 0,     // Successful execution, positive response
    CC_ERROR = 1,  // Processing ended with an error
    CC_ILL = 2,    // Command cannot be executed due to lack of permission
    CC_DEL = 3,    // Command successfully deleted
    CC_NFOUND = 4, // Command for deletion not found
    CC_NCONF = 5,  // Successful execution, negative response
    CC_INPROG = 6  // Command is being processed, result is not yet known
};

enum class CommandType : uint8_t
{
    CT_COMCONF = 1, // Confirmation of command reception and processing
    // CT_MSGCONF = 2, // Confirmation of message reception and processing
    // CT_MSGFROM = 3, // Informational message from ASN
    // CT_MSGTO = 4,   // Informational message for display on ASN
    CT_COM = 5, // Command for execution on ASN
    // CT_DELCOM = 6,  // Deletion from the execution queue
    // CT_SUBREQ = 7,  // Additional sub-request for execution
    // CT_DELIV = 8    // Confirmation of command or message delivery
};

const size_t max_command_length = 65205;
const size_t max_command_data_length = 65200;

// The subrecord for type EGTS_SR_COMMAND_DATA.
struct SrCommandData
{
  private:
    frame_buffer_type m_authorization_code;

  public:
    ConfirmationType confirmation_type; // The confirmation type from command flags

    CommandType command_type; // The command type from command flags

    uint32_t command_identifier; // Identifier for the command

    uint32_t source_identifier; // Identifier for the source of the command

    CharSet charset; // Character set used in the command

    // Authorization code for command execution
    void authorization_code(frame_buffer_type);

    // Authorization code for command execution
    payload_type
    authorization_code() const noexcept;

    struct Data // command data
    {
      private:
        frame_buffer_type m_data;
        uint8_t m_size;

      public:
        friend SrCommandData;

        uint16_t address;

        void
        size(double);

        double
        size() const noexcept;

        DataAct action;

        uint16_t code;

        // Authorization code for command execution
        void data(frame_buffer_type);

        // Authorization code for command execution
        payload_type
        data() const noexcept;
    };
    Data data;

    void parse(payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
