#pragma once

#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <cstdint> // uint16_t, uint8_t
#include <error/error.h>
#include <globals.h>
#include <optional>
#include <string_view>

namespace egts::v1::record::subrecord
{

using buffer_type = egts::v1::buffer_type;

using payload_type = egts::v1::payload_type;

using uint16_t = std::uint16_t;

using uint8_t = std::uint8_t;

using Error = egts::v1::error::Error;

using Code = egts::v1::error::Code;

const size_t max_file_data_size = 65400;
const size_t max_file_name_size = 64;

// the type of entity based on what it contains
enum class ObjectType : uint8_t
{
    FIRMWARE = 0,
    CONFIG = 1
};

// the type of module that the transmitted entity is intended for
enum class ModuleType : uint8_t
{
    PERIPHERALS = 0,
    DEVICE = 1
};

// header containing parameters that characterize the transmitted entity.
// This header is transmitted only for the first part of the entity.
// When transmitting the second and subsequent parts, this field is not transmitted.
struct ObjectDataHeader
{
  private:
    std::string m_file_name{};

  public:
    ObjectType object_type{ObjectType::FIRMWARE};
    ModuleType module_type{ModuleType::DEVICE};
    // the component number if the entity belongs directly to the DEVICE,
    // or the identifier of the peripheral module/port connected to the PERIPHERALS,
    // depending on the value of the module_type parameter
    uint8_t component_or_module_identifier{};
    // version
    //
    // higher byte – the number before the dot – is the major version,
    // lower byte – after the dot – is the minor version;
    //
    // for example, version 2.34 would be represented as the number 0x0222.
    uint16_t version{};
    uint16_t crc_value{};

    std::string_view
    file_name() const noexcept;

    void file_name(std::string_view);

    void
    parse(payload_type, payload_type::iterator &);

    buffer_type
    buffer() const noexcept;
};

const uint16_t begin_part_number = 1;

// The subrecord for type EGTS_SR_SERVICE_PART_DATA.
//
// The values of the parameters "expected_parts_quantity" and "part_number" for this subrecord
// must be in the range from 1 to 65535, and the value in the "part_number" field must be less
// than or equal to the value in the "expected_parts_quantity" field.
// If this condition is violated, the data from such a subrecord will be ignored.
struct SrPartData
{
  private:
    std::optional<ObjectDataHeader> m_odh;
    buffer_type mp_data;

  public:
    // unique identifier for the transmitted entity
    uint16_t identity{};
    // sequential number of the current part of the transmitted entity
    uint16_t part_number{begin_part_number};
    // expected quantity of parts for the transmitted entity
    uint16_t expected_parts_quantity{};

    SrPartData();
    
    SrPartData(payload_type);

    void
    odh(ObjectDataHeader &&);

    ObjectDataHeader
    odh() const;

    ObjectDataHeader &&
    odh();

    // data of the transmitted entity
    void data(buffer_type);

    // data of the transmitted entity
    payload_type
    data() const noexcept;

    buffer_type
    data();

    void parse(payload_type);

    buffer_type
    buffer() const noexcept;
};

// The subrecord for type EGTS_SR_SERVICE_FULL_DATA.
struct SrFullData
{
  private:
    buffer_type mp_data;
    std::optional<ObjectDataHeader> m_odh;

  public:

    SrFullData();

    SrFullData(payload_type);

    void
    odh(ObjectDataHeader &&);

    ObjectDataHeader
    odh() const;

    // data of the transmitted entity
    void data(buffer_type);

    // data of the transmitted entity
    payload_type
    data() const noexcept;

    buffer_type
    data();

    void parse(payload_type);

    buffer_type
    buffer() const noexcept;
};

} // namespace egts::v1::record::subrecord

#endif
