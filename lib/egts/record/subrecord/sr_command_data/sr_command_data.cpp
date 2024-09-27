#include "sr_command_data.h"
#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <span>
#include <utility>

namespace egts::v1::record::subrecord
{

void
SrCommandData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 10))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    uint8_t command_flags = static_cast<uint8_t>(*ptr++);      // 0
                                                               //
    command_identifier = static_cast<uint32_t>(*ptr++) |       // 1
                         static_cast<uint32_t>(*ptr++) << 8 |  // 2
                         static_cast<uint32_t>(*ptr++) << 16 | // 3
                         static_cast<uint32_t>(*ptr++) << 24;  // 4
                                                               //
    source_identifier = static_cast<uint32_t>(*ptr++) |        // 5
                        static_cast<uint32_t>(*ptr++) << 8 |   // 6
                        static_cast<uint32_t>(*ptr++) << 16 |  // 7
                        static_cast<uint32_t>(*ptr++) << 24;   // 8
                                                               //
    uint8_t flag = static_cast<uint8_t>(*ptr++);               // 9

    confirmation_type = static_cast<ConfirmationType>(command_flags & 0x0F);
    command_type = static_cast<CommandType>(command_flags >> 4);

    if (flag & 0x01) // CHSFE
    {
        if (!has_remaining_bytes(buffer, ptr, 1))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        charset = static_cast<CharSet>(*ptr++);
    }

    if (flag & 0x02) // ACFE
    {
        if (!has_remaining_bytes(buffer, ptr, 1))
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }

        auto length = static_cast<uint8_t>(*ptr++); // ACL
        if (length > 0)
        {
            if (!has_remaining_bytes(buffer, ptr, length))
            {
                throw Error(Code::EGTS_PC_INVDATALEN);
            }
            m_authorization_code.assign(ptr, ptr + length);
            ptr += length;
        }
    }

    size_t data_size = std::distance(ptr, buffer.end());
    if (data_size > max_command_length)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    if (data_size > 0) // if then parse Command Data
    {
        data.address = static_cast<uint16_t>(*ptr++) |     // 0
                       static_cast<uint16_t>(*ptr++) << 8; // 1
                                                           //
        uint8_t flag = static_cast<uint8_t>(*ptr++);       // 2
                                                           //
        data.code = static_cast<uint16_t>(*ptr++) |        // 3
                    static_cast<uint16_t>(*ptr++) << 8;    // 4

        data.action = static_cast<DataAct>(flag & 0x0F);

        if (data.action == DataAct::ADD_ACT) // parse size
        {
            data.size(flag >> 4);
        }

        // TODO: data.size <= data_size ????

        data_size = std::distance(ptr, buffer.end());
        if (data_size > max_command_data_length)
        {
            throw Error(Code::EGTS_PC_INVDATALEN);
        }
        data.m_data.assign(ptr, buffer.end());
    }
}

void
SrCommandData::authorization_code(frame_buffer_type code)
{
    if (code.size() > std::numeric_limits<uint8_t>::max())
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    m_authorization_code = std::move(code);
};

payload_type
SrCommandData::authorization_code() const noexcept
{
    return m_authorization_code;
}

void
SrCommandData::Data::data(frame_buffer_type data)
{
    if (data.size() > max_command_data_length)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    m_data = std::move(data);
};

payload_type
SrCommandData::Data::data() const noexcept
{
    return m_data;
}

void
SrCommandData::Data::size(double size)
{
    if (size <= 0)
    {
        throw std::runtime_error("size must be greater than 0");
    }
    m_size = static_cast<uint8_t>(std::ceil(std::log2(size)));
};

double
SrCommandData::Data::size() const noexcept
{
    return std::pow(2, static_cast<double>(m_size));
}

frame_buffer_type
SrCommandData::buffer() const noexcept
{
    size_t size{10 + 5};
    uint8_t flag{0};
    if (charset > CharSet::CP1251)
    {
        flag |= 0x01; // CHSFE
        size += 1;
    }

    if (!m_authorization_code.empty())
    {
        flag |= 0x02; // ACFE
        size += 1 + m_authorization_code.size();
    }

    if (!data.m_data.empty())
    {
        size += data.m_data.size();
    }

    frame_buffer_type buffer(size, 0);
    auto ptr = buffer.begin();

    *ptr++ = static_cast<uint8_t>((static_cast<uint8_t>(command_type) << 4) | (static_cast<uint8_t>(confirmation_type) & 0x0f)); // 0
                                                                                                                                 //
    *ptr++ = static_cast<uint8_t>(command_identifier);                                                                           // 1
    *ptr++ = static_cast<uint8_t>(command_identifier >> 8);                                                                      // 2
    *ptr++ = static_cast<uint8_t>(command_identifier >> 16);                                                                     // 3
    *ptr++ = static_cast<uint8_t>(command_identifier >> 24);                                                                     // 4
                                                                                                                                 //
    *ptr++ = static_cast<uint8_t>(source_identifier);                                                                            // 5
    *ptr++ = static_cast<uint8_t>(source_identifier >> 8);                                                                       // 6
    *ptr++ = static_cast<uint8_t>(source_identifier >> 16);                                                                      // 7
    *ptr++ = static_cast<uint8_t>(source_identifier >> 24);                                                                      // 8
                                                                                                                                 //
    *ptr++ = static_cast<uint8_t>(flag);                                                                                         // 9

    if (charset > CharSet::CP1251)
    {
        *ptr++ = static_cast<uint8_t>(charset);
    }

    if (!m_authorization_code.empty())
    {
        *ptr++ = static_cast<uint8_t>(m_authorization_code.size());
        std::copy(m_authorization_code.begin(), m_authorization_code.end(), ptr);
    }

    flag = (data.m_size << 4) | static_cast<uint8_t>(data.action);

    *ptr++ = static_cast<uint8_t>(data.address);      // 0
    *ptr++ = static_cast<uint8_t>(data.address >> 8); // 1
    *ptr++ = static_cast<uint8_t>(flag);              // 2
    *ptr++ = static_cast<uint8_t>(data.code);         // 3
    *ptr++ = static_cast<uint8_t>(data.code >> 8);    // 4
    std::copy(data.m_data.begin(), data.m_data.end(), ptr);
    return buffer;
}

} // namespace egts::v1::record::subrecord