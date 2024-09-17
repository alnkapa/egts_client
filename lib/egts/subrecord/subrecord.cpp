#include "subrecord.h"
#include <iterator>
#include <span>

namespace egts::v1::record::subrecord
{

error::Error
SubRecord::parse(record_payload_type, record_payload_type::iterator &) noexcept
{
    return error::Error();
}

}