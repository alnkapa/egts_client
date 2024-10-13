#pragma once
#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include "i_file_model.h"
#include <QFile>
#include <QString>

namespace model
{

class File : public IFile, public QObject
{
    Q_OBJECT

  public:
    File(const QString &file_name, QObject *parent = nullptr);

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
    is_open() override;

    void
    close() override;

    bool
    eof() override;

  private:
    QFile m_file;
};

} // namespace model
#endif
