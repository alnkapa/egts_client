#include "my_globals.h"
#include <nmea/message/gga.hpp>
#include <nmea/sentence.hpp>
#include <string>
#include <string_view>

void
my_parse_string(std::string_view str)
{
    try
    {
        nmea::sentence sentence(str.data());
        if (sentence.type() == "GGA")
        {
            nmea::gga gga(sentence);

            std::cout << "UTC: " << std::fixed << std::setprecision(2) << gga.utc.get() << std::endl;

            if (gga.latitude.exists())
            {
                std::cout << "Latitude: " << std::fixed << std::setprecision(6) << gga.latitude.get() << std::endl;
            }

            if (gga.longitude.exists())
            {
                std::cout << "Longitude: " << std::fixed << std::setprecision(6) << gga.longitude.get() << std::endl;
            }
        }
    }
    catch (const std::exception &err)
    {
        std::cerr << "nmea: error: " << err.what() << std::endl;
    }
}

void
my_read_file(std::shared_ptr<std::ifstream> file) noexcept
{
    std::string line;
    while (g_keep_running)
    {
        try
        {
            std::cerr << "read_file: begin " << file->tellg() << std::endl;
            while (g_keep_running && std::getline(*file, line))
            {
                my_parse_string(line);
            }
            std::cerr << "read_file: end " << file->tellg() << std::endl;
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