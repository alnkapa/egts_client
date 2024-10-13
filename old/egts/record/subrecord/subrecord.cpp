#include "subrecord.h"
#include <ostream>

namespace egts::v1::record::subrecord
{

void
SubRecord::parse(payload_type buffer, payload_type::iterator &ptr)
{
    auto begin = ptr;

    if (!has_remaining_bytes(buffer, ptr, 3))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    m_type = static_cast<Type>(*ptr++);                 // 0
    m_length = static_cast<std::uint16_t>(*ptr++) |     // 1
               static_cast<std::uint16_t>(*ptr++) << 8; // 2

    if (m_length > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }

    if (!has_remaining_bytes(buffer, ptr, m_length))
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto offset = std::distance(buffer.begin(), begin);
    offset += std::distance(begin, ptr);
    mp_data = buffer.subspan(offset, m_length);
    ptr += m_length;

#ifdef MY_DEBUG
    std::cout << "parse sub record type: " << m_type << " length: " << static_cast<int>(m_length) << "\ndata: " << mp_data << std::endl;
#endif
}

Type
SubRecord::type() const
{
    return m_type;
}

payload_type
SubRecord::data() const
{
    return mp_data;
}

uint16_t
SubRecord::length() const
{
    return m_length;
}

buffer_type
wrapper(Type type, buffer_type &&data)
{
    if (data.size() > max_frame_size)
    {
        throw Error(Code::EGTS_PC_INVDATALEN);
    }
    auto record_length = data.size();

    const std::size_t size = 3;

    buffer_type buffer(size, 0);
    buffer.reserve(size + record_length);

    auto ptr = buffer.begin();
    *ptr++ = static_cast<uint8_t>(type);               // 0
    *ptr++ = static_cast<uint8_t>(record_length);      // 1
    *ptr++ = static_cast<uint8_t>(record_length >> 8); // 2
    buffer.insert(
        ptr,
        std::make_move_iterator(data.begin()),
        std::make_move_iterator(data.end()));

#ifdef MY_DEBUG
    std::cout << "wrapper sub record: " << type << " length: " << static_cast<int>(record_length) << "\ndata: " << buffer << std::endl;
#endif
    return buffer;
}

} // namespace egts::v1::record::subrecord

std::ostream &
operator<<(std::ostream &os, const egts::v1::record::subrecord::Type &type)
{
    using egts::v1::record::subrecord::Type;
    switch (type)
    {
    case Type::EGTS_SR_RECORD_RESPONSE:
        os << "EGTS_SR_RECORD_RESPONSE";
        break;
    case Type::EGTS_SR_TERM_IDENTITY:
        os << "EGTS_SR_TERM_IDENTITY";
        break;
    case Type::EGTS_SR_MODULE_DATA:
        os << "EGTS_SR_MODULE_DATA";
        break;
    case Type::EGTS_SR_VEHICLE_DATA:
        os << "EGTS_SR_VEHICLE_DATA";
        break;
    case Type::EGTS_SR_DISPATCHER_IDENTITY:
        os << "EGTS_SR_DISPATCHER_IDENTITY";
        break;
    case Type::EGTS_SR_AUTH_PARAMS:
        os << "EGTS_SR_AUTH_PARAMS";
        break;
    case Type::EGTS_SR_AUTH_INFO:
        os << "EGTS_SR_AUTH_INFO";
        break;
    case Type::EGTS_SR_SERVICE_INFO:
        os << "EGTS_SR_SERVICE_INFO";
        break;
    case Type::EGTS_SR_RESULT_CODE:
        os << "EGTS_SR_RESULT_CODE";
        break;
    case Type::EGTS_SR_POS_DATA:
        os << "EGTS_SR_POS_DATA";
        break;
    case Type::EGTS_SR_EXT_POS_DATA:
        os << "EGTS_SR_EXT_POS_DATA";
        break;
    case Type::EGTS_SR_AD_SENSORS_DATA:
        os << "EGTS_SR_AD_SENSORS_DATA";
        break;
    case Type::EGTS_SR_COUNTERS_DATA:
        os << "EGTS_SR_COUNTERS_DATA";
        break;
    case Type::EGTS_SR_STATE_DATA:
        os << "EGTS_SR_STATE_DATA";
        break;
    case Type::EGTS_SR_ACCEL_DATA:
        os << "EGTS_SR_ACCEL_DATA";
        break;
    case Type::EGTS_SR_LOOPIN_DATA:
        os << "EGTS_SR_LOOPIN_DATA";
        break;
    case Type::EGTS_SR_ABS_DIG_SENS_DATA:
        os << "EGTS_SR_ABS_DIG_SENS_DATA";
        break;
    case Type::EGTS_SR_ABS_AN_SENS_DATA:
        os << "EGTS_SR_ABS_AN_SENS_DATA";
        break;
    case Type::EGTS_SR_ABS_CNTR_DATA:
        os << "EGTS_SR_ABS_CNTR_DATA";
        break;
    case Type::EGTS_SR_ABS_LOOPIN_DATA:
        os << "EGTS_SR_ABS_LOOPIN_DATA";
        break;
    case Type::EGTS_SR_LIQUID_LEVEL_SENSOR:
        os << "EGTS_SR_LIQUID_LEVEL_SENSOR";
        break;
    case Type::EGTS_SR_PASSENGERS_COUNTERS:
        os << "EGTS_SR_PASSENGERS_COUNTERS";
        break;
    case Type::EGTS_SR_SERVICE_PART_DATA:
        os << "EGTS_SR_SERVICE_PART_DATA";
        break;
    case Type::EGTS_SR_SERVICE_FULL_DATA:
        os << "EGTS_SR_SERVICE_FULL_DATA";
        break;
    case Type::EGTS_SR_COMMAND_DATA:
        os << "EGTS_SR_COMMAND_DATA";
        break;
    default:
        os << "Unknown Type";
        break;
    }
    return os;
}
