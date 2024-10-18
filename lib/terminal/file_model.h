#pragma once

#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include "i_file_model.h"
#include <fstream>
#include <string>

namespace model
{

class File : public IFile
{

  public:
    File(const std::string &file_name);

    virtual ~File();

    std::size_t
    write(std::uint8_t *ptr, std::size_t size) override;

    std::vector<std::uint8_t>
    read(std::size_t size = 0) override;

    void
    seek(std::size_t offset = 0) override;

    std::size_t
    tell() override;

    bool
    eof() override;

  private:
    std::fstream m_file;
    std::size_t m_size{0};
};

} // namespace model
#endif
