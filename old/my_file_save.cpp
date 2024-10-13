#include "crc.h"
#include "firmware/firmware.h"
#include "my_globals.h"
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>

void
FileHolder::add_part(egts::v1::record::subrecord::SrPartData &&in)
{
    std::lock_guard lock(lo);
    if (in.part_number == egts::v1::record::subrecord::begin_part_number)
    {
        m_value.emplace(
            in.identity,
            value{
                in.expected_parts_quantity,
                std::move(in.odh()),
                std::move(in.data()),
            });
    }
    else
    {
        auto it = m_value.find(in.identity);
        if (it == m_value.end())
        {
            throw std::runtime_error("file part not found");
        }
        it->second.data += in.data();

        if (it->second.expected_parts_quantity == in.part_number) // file download
        {
            std::uint16_t crc_value = egts::v1::crc16(
                it->second.data.begin(),
                it->second.data.end());
            if (crc_value != it->second.odh.crc_value)
            {
                throw std::runtime_error("crc not equal");
            }
            save_to_disc(std::move(it->second));
        }
    }
}

void
FileHolder::add_full(egts::v1::record::subrecord::SrFullData &&in)
{
    save_to_disc(
        value{
            0,
            std::move(in.odh()),
            std::move(in.data()),
        });
}

void
FileHolder::save_to_disc(value &&in)
{
    std::fstream file(
        in.odh.file_name().data(),
        std::ios_base::in |
            std::ios_base::out |
            std::ios_base::trunc |
            std::ios_base::binary);
    if (!file.is_open())
    {
        throw std::runtime_error(std::string("error open file:") + in.odh.file_name().data());
    }
    for (const auto &v : in.data)
    {
        file.put(v);
    }
    std::cout << "file download " << in.odh.file_name() << " ok \n";
}
