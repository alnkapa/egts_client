#pragma once
#ifndef PRIORITY_HPP
#define PRIORITY_HPP

#include <string>

namespace egts
{
    namespace transport
    {
        /**
         * routing priority
         */
        enum class Priority : unsigned char
        {
            highest = 0b00,
            high = 0b01,
            middle = 0b10,
            low = 0b11,
        };
        std::string getPriority(Priority p)
        {
            switch (p)
            {
            case Priority::highest:
                return "highest";
            case Priority::high:
                return "high";
            case Priority::middle:
                return "middle";
            case Priority::low:
                return "low";
            }
            return "???";
        };
    }
}
#endif /* PRIORITY_HPP */