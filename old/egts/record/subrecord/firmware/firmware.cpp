#include "firmware.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <span>
#include <stdexcept>
#include <utility>

namespace egts::v1::record::subrecord
{

std::string_view
ObjectDataHeader::file_name() const noexcept
{
    return m_file_name;
}

void
ObjectDataHeader::file_name(std::string_view in)
{
    if (in.size() > max_file_name_size)
    {
        throw std::runtime_error("file name max size 64");
    }
    m_file_name = in;
}

void
ObjectDataHeader::parse(payload_type buffer, payload_type::iterator &ptr)
{
    if (!has_remaining_bytes(buffer, ptr, 6))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    uint8_t flag = static_cast<uint8_t>(*ptr++);                   // 0
                                                                   //
    component_or_module_identifier = static_cast<uint8_t>(*ptr++); // 1
                                                                   //
    version = static_cast<uint16_t>(*ptr++) |                      // 2
              static_cast<uint16_t>(*ptr++) << 8;                  // 3
                                                                   //
    crc_value = static_cast<uint16_t>(*ptr++) |                    // 4
                static_cast<uint16_t>(*ptr++) << 8;                // 5

    module_type = static_cast<ModuleType>(flag & 0x03);
    object_type = static_cast<ObjectType>((flag >> 2) & 0x03);

    auto zero_it = std::find(ptr, buffer.end(), 0); // find Delimiter
    if (zero_it == buffer.end())
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    else
    {
        size_t size = std::distance(ptr, zero_it);
        if (size > max_file_name_size)
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        else if (size > 0)
        {
            m_file_name.assign(ptr, zero_it);
        }
        else
        {
            m_file_name.clear();
        }
    }
    ptr = zero_it + 1;
}

buffer_type
ObjectDataHeader::buffer() const noexcept
{
    size_t size = 6 + 1;
    if (m_file_name.size() > 0)
    {
        size += m_file_name.size();
    }

    buffer_type buffer(size, 0);
    auto ptr = buffer.begin();
    *ptr++ = ((static_cast<uint8_t>(object_type) & 0x03) << 2) | (static_cast<uint8_t>(module_type) & 0x03); // 0
                                                                                                             //
    *ptr++ = static_cast<uint8_t>(component_or_module_identifier);                                           // 1
                                                                                                             //
    *ptr++ = static_cast<uint8_t>(version);                                                                  // 2
    *ptr++ = static_cast<uint8_t>(version >> 8);                                                             // 3
                                                                                                             //
    *ptr++ = static_cast<uint8_t>(crc_value);                                                                // 4
    *ptr++ = static_cast<uint8_t>(crc_value >> 8);                                                           // 5

    if (m_file_name.size() > 0)
    {
        std::copy(m_file_name.begin(), m_file_name.end(), ptr);
        ptr += m_file_name.size();
    }
    *ptr++ = 0; // Delimiter
    return buffer;
}

void
SrPartData::odh(ObjectDataHeader &&in)
{
    m_odh.emplace(in);
}

ObjectDataHeader
SrPartData::odh() const
{
    if (!m_odh.has_value())
    {
        throw std::runtime_error("odh absent");
    }
    return m_odh.value();
}

ObjectDataHeader &&
SrPartData::odh()
{
    if (!m_odh.has_value())
    {
        throw std::runtime_error("odh absent");
    }
    return std::move(m_odh.value());
}

void
SrPartData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 6))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    identity = static_cast<uint16_t>(*ptr++) |                    // 0
               static_cast<uint16_t>(*ptr++) << 8;                // 1
                                                                  //
    part_number = static_cast<uint16_t>(*ptr++) |                 // 2
                  static_cast<uint16_t>(*ptr++) << 8;             // 3
                                                                  //
    expected_parts_quantity = static_cast<uint16_t>(*ptr++) |     // 4
                              static_cast<uint16_t>(*ptr++) << 8; // 5

    if (m_odh.has_value())
    {
        m_odh.reset();
    }

    if (part_number == begin_part_number)
    {
        ObjectDataHeader odh;
        odh.parse(buffer, ptr);
        m_odh.emplace(std::move(odh));
    }

    size_t size = std::distance(ptr, buffer.end());
    if (size > max_file_data_size || size == 0)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    mp_data.resize(size);
    std::copy(ptr, buffer.end(), mp_data.begin());
}

buffer_type
SrPartData::buffer() const noexcept
{
    size_t size = 6;
    buffer_type buffer(size, 0);
    buffer_type odh_buffer;
    if (part_number == begin_part_number && m_odh.has_value())
    {
        odh_buffer = m_odh->buffer();
        buffer.reserve(size + odh_buffer.size() + mp_data.size());
    }
    else
    {
        buffer.reserve(size + mp_data.size());
    }

    auto ptr = buffer.begin();

    *ptr++ = static_cast<uint8_t>(identity);                     // 0
    *ptr++ = static_cast<uint8_t>(identity >> 8);                // 1
                                                                 //
    *ptr++ = static_cast<uint8_t>(part_number);                  // 2
    *ptr++ = static_cast<uint8_t>(part_number >> 8);             // 3
                                                                 //
    *ptr++ = static_cast<uint8_t>(expected_parts_quantity);      // 4
    *ptr++ = static_cast<uint8_t>(expected_parts_quantity >> 8); // 5

    if (part_number == begin_part_number && m_odh.has_value())
    {
        buffer.insert(
            ptr,
            std::make_move_iterator(odh_buffer.begin()),
            std::make_move_iterator(odh_buffer.end()));
        ptr += odh_buffer.size();
    }

    buffer.insert(
        ptr,
        std::make_move_iterator(mp_data.begin()),
        std::make_move_iterator(mp_data.end()));

    return buffer;
}

void
SrPartData::data(buffer_type in)
{
    mp_data = std::move(in);
}

payload_type
SrPartData::data() const noexcept
{
    return mp_data;
}

buffer_type
SrPartData::data() 
{
    return mp_data;
}

void
SrFullData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    ObjectDataHeader odh;
    odh.parse(buffer, ptr);
    m_odh.emplace(std::move(odh));

    size_t size = std::distance(ptr, buffer.end());
    if (size > max_file_data_size || size == 0)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    mp_data.resize(size);
    std::copy(ptr, buffer.end(), mp_data.begin());
}

buffer_type
SrFullData::buffer() const noexcept
{
    buffer_type buffer{};
    if (m_odh.has_value())
    {
        buffer_type odh_buffer = m_odh.value().buffer();
        buffer.reserve(odh_buffer.size() + mp_data.size());
        buffer.insert(
            buffer.end(),
            std::make_move_iterator(odh_buffer.begin()),
            std::make_move_iterator(odh_buffer.end()));
    }
    else
    {
        buffer.reserve(mp_data.size());
    }
    buffer.insert(
        buffer.end(),
        std::make_move_iterator(mp_data.begin()),
        std::make_move_iterator(mp_data.end()));
    return buffer;
}

void
SrFullData::odh(ObjectDataHeader &&in)
{
    m_odh.emplace(in);
}

ObjectDataHeader
SrFullData::odh() const
{
    if (!m_odh.has_value())
    {
        throw std::runtime_error("odh absent");
    }
    return m_odh.value();
}

void
SrFullData::data(buffer_type in)
{
    mp_data = std::move(in);
}

payload_type
SrFullData::data() const noexcept
{
    return mp_data;
}

buffer_type
SrFullData::data() 
{
    return mp_data;
}


} // namespace egts::v1::record::subrecord