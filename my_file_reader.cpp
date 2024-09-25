#include "my_globals.h"
#include "nmea/object/status.hpp"
#include <chrono>
#include <cstdint>
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

uint32_t
convert(double lat)
{
    auto degrees = static_cast<uint32_t>(lat / 100);
    auto minutes = static_cast<uint32_t>(lat * 10000) % 1000000;
    return (degrees * 10000000) + (minutes * 100 / 6);
}

bool
my_parse_string(std::string_view str, egts::v1::record::subrecord::SrPosData &rd)
{
    bool ready_for_send{false};
    try
    {
        nmea::sentence sentence(str.data());
        if (sentence.type() == "GGA") // Global Positioning System Fix Data
        {
            nmea::gga gga(sentence);
            rd.altitude = static_cast<uint16_t>(gga.altitude.get());
            rd.flags |= 1 << 7; // ALTE

            // gga.satellite_count.get();
            // gga.hdop.get() * 10;

            switch (gga.fix.get())
            {
            case nmea::gga::fix_type::DGPS:
                rd.flags |= 1 << 1; // FIX
                break;
            default:
                break;
            }
        }
        else if (sentence.type() == "GSV") // GPS Satellites in View
        {
            nmea::gsv gsv(sentence);
            gsv.satellite_count.get();
        }
        else if (sentence.type() == "RMC") // Recommended Minimum Specific GPS/Transit Data
        {
            nmea::rmc rmc(sentence);
            if (rmc.latitude.get() < 0) // S
            {
                rd.flags |= 1 << 5; // LAHS
            }
            rd.latitude = convert(rmc.latitude.get());
            if (rmc.longitude.get() < 0) // W
            {
                rd.flags |= 1 << 6; // LOHS
            }
            rd.longitude = convert(rmc.longitude.get());
            rd.speed = static_cast<uint16_t>(rmc.speed.get() * 1.85200);
            rd.direction = static_cast<uint8_t>(rmc.track_angle.get());
            // TODO: DIRH ??
            if (rmc.status.get() == nmea::status::ACTIVE)
            {
                rd.flags |= 1; // VLD
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
    egts::v1::record::subrecord::SrPosData rd{};
    int count{0};
    while (g_keep_running)
    {
        try
        {
            while (g_keep_running && std::getline(*file, line))
            {
                if (my_parse_string(line, rd))
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
                    if (count < 10)
                    {
                        g_send_queue.push(std::move(new_pkg));
#ifdef MY_DEBUG
                        std::cout << "line:" << line << std::endl;
#endif
                        // g_keep_running = false;
                    }
                    count++;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
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