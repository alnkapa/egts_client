#include "record.h"

namespace egts::v1::record
{
Record::Record(uint16_t record_number)
{
}

Record::~Record()
{
}

error::Error
Record::parse(std::vector<unsigned char> &&buffer) noexcept
{
    return error::Error();
}
}

