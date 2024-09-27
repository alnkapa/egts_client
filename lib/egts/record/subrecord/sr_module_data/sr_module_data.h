#pragma once
#ifndef SR_MODULE_DATA_H
#define SR_MODULE_DATA_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>

namespace egts::v1::record::subrecord
{

using frame_buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint32_t = std::uint32_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

// The subrecord for type EGTS_SR_MODULE_DATA.
struct SrModuleData
{
    // module_type - defines the functional affiliation of the module
    // 1 - main module;
    // 2 - input-output module;
    // 3 - navigation receiver module;
    // 4 - wireless communication module;
    // Recommended numbering rules for module types are specified here.
    // The specific implementation of the Authorization Service may introduce and expand its own numbering of types,
    // including all external peripheral controllers.
    uint8_t module_type;

    // Vendor identifier
    uint32_t vendor_identifier;

    // Hardware version of the module
    uint16_t firmware_version;

    // Software version of the module
    // (the high byte - number before the dot - major version,
    // the low byte - number after the dot - minor version,
    // for example, version 2.34 will be represented as 0x0222);
    uint16_t software_version;

    // Modification code of the software part of the module;
    // (the high byte - number before the dot, the low byte - number after the dot);
    uint8_t modification;

    // State
    // 1 - enabled,
    // 0 - disabled,
    // >127 - malfunction, see result codes;
    uint8_t state;

    // Serial number of the module
    //
    // TODO: replace '0' to '_'
    std::string serial_number;

    // Brief description of the module
    //
    // TODO: replace '0' to '_'
    std::vector<std::string> description;

    void parse(payload_type);

    frame_buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
