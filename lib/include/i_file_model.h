#pragma once
#ifndef I_FILE_MODEL_H
#define I_FILE_MODEL_H

#include <cstdint>
#include <vector>

namespace model
{

class IFile
{
  public:
    virtual ~IFile() = default;

    virtual std::size_t
    write(std::uint8_t *ptr, std::size_t size) = 0;

    virtual std::vector<std::uint8_t>
    read(std::size_t size = 0) = 0;

    virtual void
    seek(std::size_t offset = 0) = 0;

    virtual std::size_t
    tell() = 0;

    virtual bool
    is_open() = 0;

    virtual void
    close() = 0;

    virtual bool
    eof() = 0;
};

} // namespace model
#endif
