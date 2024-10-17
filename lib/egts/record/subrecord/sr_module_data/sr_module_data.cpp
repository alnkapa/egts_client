#include "sr_module_data.h"
#include <algorithm>
#include <cstddef>
#include <iterator>

namespace egts::v1::record::subrecord
{

void
SrModuleData::parse(payload_type buffer)
{
    auto ptr = buffer.begin();
    if (!has_remaining_bytes(buffer, ptr, 11))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    module_type = static_cast<uint8_t>(*ptr++);               // 0
                                                              //
    vendor_identifier = static_cast<uint32_t>(*ptr++) |       // 1
                        static_cast<uint32_t>(*ptr++) << 8 |  // 2
                        static_cast<uint32_t>(*ptr++) << 16 | // 3
                        static_cast<uint32_t>(*ptr++) << 24;  // 4
                                                              //
    firmware_version = static_cast<uint32_t>(*ptr++) |        // 5
                       static_cast<uint32_t>(*ptr++) << 8;    // 6
                                                              //
    software_version = static_cast<uint32_t>(*ptr++) |        // 7
                       static_cast<uint32_t>(*ptr++) << 8;    // 8
                                                              //
    modification = static_cast<uint8_t>(*ptr++);              // 9
                                                              //
    state = static_cast<uint8_t>(*ptr++);                     // 10

    serial_number.clear();
    description.clear();

    size_t counter{0};
    while (ptr != buffer.end())
    {
        auto zero_it = std::find(ptr, buffer.end(), 0); // find Delimiter
        if (zero_it != buffer.end())
        {
            if (counter == 0) // serial_number
            {
                serial_number.assign(ptr, zero_it);
            }
            else // description
            {
                description.emplace_back(std::string(ptr, zero_it));
            }
            ptr = ++zero_it;
            counter++;
        }
        else
        {
            break;
        }
    }
}

buffer_type
SrModuleData::buffer() const noexcept
{
    size_t ext_size = 0;

    if (!serial_number.empty())
    {
        ext_size += serial_number.size() + 1; // +Delimiter
    }

    if (!description.empty())
    {
        for (const auto &v : description)
        {
            ext_size += v.size() + 1; // +Delimiter
        }
    }

    buffer_type buffer(11 + ext_size, 0);
    auto ptr = buffer.begin();

    *ptr++ = static_cast<uint8_t>(module_type);             // 0
                                                            //
    *ptr++ = static_cast<uint8_t>(vendor_identifier);       // 1
    *ptr++ = static_cast<uint8_t>(vendor_identifier >> 8);  // 2
    *ptr++ = static_cast<uint8_t>(vendor_identifier >> 16); // 3
    *ptr++ = static_cast<uint8_t>(vendor_identifier >> 24); // 4
                                                            //
    *ptr++ = static_cast<uint8_t>(firmware_version);        // 5
    *ptr++ = static_cast<uint8_t>(firmware_version >> 8);   // 6
                                                            //
    *ptr++ = static_cast<uint8_t>(software_version);        // 7
    *ptr++ = static_cast<uint8_t>(software_version >> 8);   // 8
                                                            //
    *ptr++ = static_cast<uint8_t>(modification);            // 9
                                                            //
    *ptr++ = static_cast<uint8_t>(state);                   // 10

    if (!serial_number.empty())
    {
        std::copy(serial_number.begin(), serial_number.end(), ptr); // add serial_number
        ptr += serial_number.size();
        *ptr++ = static_cast<uint8_t>(0); // Delimiter
    }

    if (!description.empty())
    {
        for (const auto &v : description)
        {
            std::copy(v.begin(), v.end(), ptr); // add description
            ptr += v.size();
            *ptr++ = static_cast<uint8_t>(0); // Delimiter
        }
    }

    return buffer;
}

} // namespace egts::v1::record::subrecord