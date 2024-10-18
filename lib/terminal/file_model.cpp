#include "file_model.h"
#include <algorithm>
#include <cstddef>

namespace model
{

File::File(const std::string &file_name)
    : m_file(file_name, std::ios::in | std::ios::out | std::ios::binary)
{
    if (!m_file.is_open())
    {
        throw std::runtime_error("Could not open file: " + file_name);
    }
    m_file.seekg(0, std::ios::end);
    m_size = m_file.tellg();
    m_file.seekg(0, std::ios::beg);
}

File::~File()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

std::size_t
File::write(std::uint8_t *ptr, std::size_t size)
{
    if (!m_file.is_open() || size == 0)
    {
        return 0;
    }
    std::size_t start_pos = m_file.tellp();
    m_file.write(reinterpret_cast<const char *>(ptr), size);
    if (m_file.fail())
    {
        return 0;
    }
    std::size_t write_size = static_cast<std::size_t>(m_file.tellp()) - start_pos;
    m_size = std::max(m_size, start_pos + write_size);
    return write_size;
}

std::vector<std::uint8_t>
File::read(std::size_t size)
{
    if (!m_file.is_open())
    {
        return {};
    }
    if (size == 0 || size > m_size)
    {
        size = m_size;
    }
    std::vector<std::uint8_t> buffer(size);
    m_file.read(reinterpret_cast<char *>(buffer.data()), size);
    if (m_file.fail())
    {
        return {};
    }
    buffer.resize(m_file.gcount());
    return buffer;
}

void
File::seek(std::size_t size)
{
    if (m_file.is_open())
    {
        m_file.seekg(size); // Setting the reading position
        m_file.seekp(size); // Setting the writing position
    }
}

std::size_t
File::tell()
{
    if (m_file.is_open())
    {
        return static_cast<std::size_t>(m_file.tellg()); // Getting the reading position
    }
    return 0;
}

bool
File::eof()
{
    if (!m_file.is_open())
    {
        return true;
    }
    return m_file.eof();
}

} // namespace model
