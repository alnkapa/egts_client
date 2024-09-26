#include "my_globals.h"
#include "nmea/object/status.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <nmea/message/gga.hpp>
#include <nmea/message/gsv.hpp>
#include <nmea/message/rmc.hpp>
#include <nmea/sentence.hpp>
#include <ostream>
#include <string>
#include <string_view>

long
get_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    return static_cast<long>(now_c - g_start_time_t);
}

constexpr double maxUint32 = std::numeric_limits<uint32_t>::max();

thread_local egts::v1::record::subrecord::SrPosData rd{};

bool
my_parse_string(std::string_view str)
{
    bool ready_for_send{false};
    try
    {
        nmea::sentence sentence(str.data());
        if (sentence.type() == "GGA") // Global Positioning System Fix Data
        {
            nmea::gga gga(sentence);
            if (gga.altitude.exists())
            {
                rd.flags |= 1 << 7; // ALTE
                if (gga.altitude.get() < 0)
                {
                    rd.flags |= 1 << 6; // ALTS
                }
                rd.altitude = static_cast<uint32_t>(gga.altitude.get()) & 0x0FFF;
            }
            else
            {
                rd.altitude = 0;
                rd.flags &= ~(1 << 7); // ALTE
                rd.flags &= ~(1 << 6); // ALTS
            }
            // // gga.satellite_count.get();
            // // gga.hdop.get() * 10;
            if (gga.fix.exists() && gga.fix.get() == nmea::gga::fix_type::DGPS)
            {
                rd.flags |= 1 << 1; // FIX
            }
            else
            {
                rd.flags &= ~(1 << 1); // FIX
            }
        }
        else if (sentence.type() == "GSV") // GPS Satellites in View
        {
            // nmea::gsv gsv(sentence);
            // gsv.satellite_count.get();
        }
        else if (sentence.type() == "RMC") // Recommended Minimum Specific GPS/Transit Data
        {
            nmea::rmc rmc(sentence);
            if (rmc.status.get() == nmea::status::ACTIVE)
            {
                rd.flags |= 1; // VLD

                if (rmc.longitude.exists() && rmc.latitude.exists() &&
                    (rmc.latitude.get() >= -90.0 && rmc.latitude.get() <= 90.0) &&
                    (rmc.longitude.get() >= -180.0 && rmc.longitude.get() <= 180.0))
                {

                    if (rmc.longitude.get() < 0) // W
                    {                            //
                        rd.flags |= 1 << 6;      // LOHS
                    }
                    rd.longitude = static_cast<uint32_t>((rmc.longitude.get() / 180.0) * maxUint32);

                    if (rmc.latitude.get() < 0) // S
                    {                           //
                        rd.flags |= 1 << 5;     // LAHS
                    }
                    rd.latitude = static_cast<uint32_t>((rmc.latitude.get() / 90.0) * maxUint32);
                }
                else
                {
                    rd.flags &= ~(1);      // VLD
                    rd.flags &= ~(1 << 5); // LAHS
                    rd.flags &= ~(1 << 6); // LOHS
                }

                if (rmc.speed.exists())
                {
                    rd.speed = static_cast<uint16_t>(rmc.speed.get() * 1.852 * 10) & 0x3FFF;
                }
                else
                {
                    rd.speed = 0;
                }

                if (rmc.track_angle.exists())
                {
                    auto direction = static_cast<uint16_t>(rmc.track_angle.get());
                    if ((direction & 0x100) != 0)
                    {
                        rd.speed |= 1 << 7; // DIRH
                    }
                    rd.direction = static_cast<uint8_t>(direction);
                }
                else
                {
                    rd.speed &= ~(1 << 7); // DIRH
                }
            }
            else
            {
                rd.flags &= ~(1); // VLD

                rd.longitude = 0;
                rd.latitude = 0;
                rd.flags &= ~(1 << 5); // LAHS
                rd.flags &= ~(1 << 6); // LOHS

                rmc.speed = 0;
                rd.direction = 0;
                rd.speed &= ~(1 << 7); // DIRH

                rd.altitude = 0;
                rd.flags &= ~(1 << 7); // ALTE
                rd.flags &= ~(1 << 6); // ALTS
            }
            rd.navigation_time = get_time();
            ready_for_send = true;
        }
    }
    catch (const std::exception &err)
    {
        std::cerr << "nmea: error: " << err.what() << std::endl;
    }
    return ready_for_send;
}

void
my_read_file(std::shared_ptr<std::ifstream> file) noexcept
{
    std::string line;
    while (g_keep_running)
    {
        try
        {
            while (g_keep_running && std::getline(*file, line))
            {
                if (my_parse_string(line))
                {

                    auto sub = egts::v1::record::subrecord::wrapper(
                        egts::v1::record::subrecord::Type::EGTS_SR_POS_DATA,
                        rd.buffer());

                    auto record_number = g_record_number++;

                    auto rec = egts::v1::record::wrapper(
                        record_number,
                        egts::v1::record::ServiceType::EGTS_TELEDATA_SERVICE,
                        egts::v1::record::ServiceType::EGTS_TELEDATA_SERVICE,
                        std::move(sub));

                    egts::v1::transport::Packet new_pkg{};
                    new_pkg.set_frame(std::move(rec));
                    g_send_queue.push(std::move(new_pkg));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                };
            }
            if (file->eof())
            {
                file->clear();
                file->seekg(0);
            }
        }
        catch (const std::exception &err)
        {
            std::cerr << "read_file: error: " << err.what() << std::endl;
            break;
        }
    }
    g_send_queue.push(Done{});
}