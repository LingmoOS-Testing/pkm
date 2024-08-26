#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>

#include "archive/Reader.h"

class FileReader : public Reader {
 public:
  FileReader(const std::string& path);
  ~FileReader();

  ssize_t read(void* buf, int32_t count, uint64_t offset) override;
  uint64_t length() override;

 private:
  int m_fd;
};

#endif
