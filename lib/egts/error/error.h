#pragma once
#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdint> // uint8_t
#include <exception>
#include <stdexcept>
#include <string>
#include <string_view>

namespace egts::v1::error
{

enum class Code : uint8_t
{
    EGTS_PC_OK = 0,
    EGTS_PC_IN_PROGRESS = 1,
    EGTS_PC_UNS_PROTOCOL = 128,
    EGTS_PC_DECRYPT_ERROR = 129,
    EGTS_PC_PROC_DENIED = 130,
    EGTS_PC_INC_HEADERFORM = 131,
    EGTS_PC_INC_DATAFORM = 132,
    EGTS_PC_UNS_TYPE = 133,
    EGTS_PC_NOTEN_PARAMS = 134,
    EGTS_PC_DBL_PROC = 135,
    EGTS_PC_PROC_SRC_DENIED = 136,
    EGTS_PC_HEADERCRC_ERROR = 137,
    EGTS_PC_DATACRC_ERROR = 138,
    EGTS_PC_INVDATALEN = 139,
    EGTS_PC_ROUTE_NFOUND = 140,
    EGTS_PC_ROUTE_CLOSED = 141,
    EGTS_PC_ROUTE_DENIED = 142,
    EGTS_PC_INVADDR = 143,
    EGTS_PC_TTLEXPIRED = 144,
    EGTS_PC_NO_ACK = 145,
    EGTS_PC_OBJ_NFOUND = 146,
    EGTS_PC_EVNT_NFOUND = 147,
    EGTS_PC_SRVC_NFOUND = 148,
    EGTS_PC_SRVC_DENIED = 149,
    EGTS_PC_SRVC_UNKN = 150,
    EGTS_PC_AUTH_DENIED = 151,
    EGTS_PC_ALREADY_EXISTS = 152,
    EGTS_PC_ID_NFOUND = 153,
    EGTS_PC_INC_DATETIME = 154,
    EGTS_PC_IO_ERROR = 155,
    EGTS_PC_NO_RES_AVAIL = 156,
    EGTS_PC_MODULE_FAULT = 157,
    EGTS_PC_MODULE_PWR_FLT = 158,
    EGTS_PC_MODULE_PROC_FLT = 159,
    EGTS_PC_MODULE_SW_FLT = 160,
    EGTS_PC_MODULE_FW_FLT = 161,
    EGTS_PC_MODULE_IO_FLT = 162,
    EGTS_PC_MODULE_MEM_FLT = 163,
    EGTS_PC_TEST_FAILED = 164,
};

class Error : public std::exception
{
  public:
    Error() noexcept
        : m_code(Code::EGTS_PC_OK), m_message(getErrorMessage(Code::EGTS_PC_OK)) {}
    Error(Code code) noexcept
        : m_code(code), m_message(getErrorMessage(code)) {}
    Error(Code code, std::string_view message) noexcept
        : m_code(code), m_message(message) {}

    // Деструктор
    ~Error() noexcept override = default;

    operator uint8_t() const noexcept
    {
        return static_cast<uint8_t>(m_code);
    }

    operator Code() const noexcept
    {
        return m_code;
    }

    bool
    OK() const noexcept
    {
        return m_code == Code::EGTS_PC_OK;
    }

    const char *
    what() const noexcept override
    {
        return m_message.c_str();
    }

  private:
    Code m_code{Code::EGTS_PC_OK};
    std::string m_message;

    std::string
    getErrorMessage(Code code) const noexcept
    {
        switch (code)
        {
        case Code::EGTS_PC_OK:
            return "Success";
        case Code::EGTS_PC_IN_PROGRESS:
            return "In progress";
        case Code::EGTS_PC_UNS_PROTOCOL:
            return "Unsupported protocol";
        case Code::EGTS_PC_DECRYPT_ERROR:
            return "Decryption error";
        case Code::EGTS_PC_PROC_DENIED:
            return "Processing denied";
        case Code::EGTS_PC_INC_HEADERFORM:
            return "Invalid header format";
        case Code::EGTS_PC_INC_DATAFORM:
            return "Invalid data format";
        case Code::EGTS_PC_UNS_TYPE:
            return "Unsupported type";
        case Code::EGTS_PC_NOTEN_PARAMS:
            return "Invalid number of parameters";
        case Code::EGTS_PC_DBL_PROC:
            return "Double processing attempt";
        case Code::EGTS_PC_PROC_SRC_DENIED:
            return "Processing from source denied";
        case Code::EGTS_PC_HEADERCRC_ERROR:
            return "Header checksum error";
        case Code::EGTS_PC_DATACRC_ERROR:
            return "Data checksum error";
        case Code::EGTS_PC_INVDATALEN:
            return "Invalid data length";
        case Code::EGTS_PC_ROUTE_NFOUND:
            return "Route not found";
        case Code::EGTS_PC_ROUTE_CLOSED:
            return "Route closed";
        case Code::EGTS_PC_ROUTE_DENIED:
            return "Routing denied";
        case Code::EGTS_PC_INVADDR:
            return "Invalid address";
        case Code::EGTS_PC_TTLEXPIRED:
            return "TTL expired";
        case Code::EGTS_PC_NO_ACK:
            return "No acknowledgment";
        case Code::EGTS_PC_OBJ_NFOUND:
            return "Object not found";
        case Code::EGTS_PC_EVNT_NFOUND:
            return "Event not found";
        case Code::EGTS_PC_SRVC_NFOUND:
            return "Service not found";
        case Code::EGTS_PC_SRVC_DENIED:
            return "Service denied";
        case Code::EGTS_PC_SRVC_UNKN:
            return "Unknown service type";
        case Code::EGTS_PC_AUTH_DENIED:
            return "Authorization denied";
        case Code::EGTS_PC_ALREADY_EXISTS:
            return "Object already exists";
        case Code::EGTS_PC_ID_NFOUND:
            return "Identifier not found";
        case Code::EGTS_PC_INC_DATETIME:
            return "Invalid date and time";
        case Code::EGTS_PC_IO_ERROR:
            return "I/O error";
        case Code::EGTS_PC_NO_RES_AVAIL:
            return "Insufficient resources";
        case Code::EGTS_PC_MODULE_FAULT:
            return "Module fault";
        case Code::EGTS_PC_MODULE_PWR_FLT:
            return "Power supply fault";
        case Code::EGTS_PC_MODULE_PROC_FLT:
            return "Microcontroller fault";
        case Code::EGTS_PC_MODULE_SW_FLT:
            return "Software fault";
        case Code::EGTS_PC_MODULE_FW_FLT:
            return "Firmware fault";
        case Code::EGTS_PC_MODULE_IO_FLT:
            return "I/O block fault";
        case Code::EGTS_PC_MODULE_MEM_FLT:
            return "Memory fault";
        case Code::EGTS_PC_TEST_FAILED:
            return "Test failed";
        default:
            return "Unknown error";
        }
    }
};

} // namespace egts::v1::error

#endif /* ERROR_HPP */
