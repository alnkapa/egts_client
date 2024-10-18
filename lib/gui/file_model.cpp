#include "file_model.h"
#include <cstddef>

namespace model
{

File::File(const QString &filePath, QObject *parent)
    : m_file(filePath), QObject(parent)
{
    if (!m_file.open(QIODevice::ReadWrite | QIODevice::Unbuffered))
    {
        throw std::runtime_error("Could not open file: " + filePath.toStdString());
    }
}

File::~File()
{
    if (m_file.isOpen())
    {
        m_file.close();
    }
}

std::size_t
File::write(std::uint8_t *ptr, std::size_t size)
{
    if (!m_file.isOpen() || size == 0)
    {
        return 0;
    }
    return m_file.write(reinterpret_cast<const char *>(ptr), size);
}

std::vector<std::uint8_t>
File::read(std::size_t size)
{
    if (!m_file.isOpen())
    {
        return {};
    }
    if (size == 0 || size > m_file.size())
    {
        size = m_file.size();
    }
    std::vector<std::uint8_t> buffer(size);
    size_t n = m_file.read(reinterpret_cast<char *>(buffer.data()), size);
    buffer.resize(n);
    return buffer;
}

void
File::seek(std::size_t size)
{
    if (m_file.isOpen())
    {
        m_file.seek(size);
    }
}

std::size_t
File::tell()
{
    if (m_file.isOpen())
    {
        return m_file.pos();
    }
    return 0;
}

bool
File::eof()
{
    if (!m_file.isOpen())
    {
        return true;
    }
    return m_file.atEnd();
}

} // namespace model
