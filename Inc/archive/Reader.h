#ifndef READER_H
#define READER_H
#include <cstdint>

class Reader {
 public:
  virtual ~Reader() = default;
  virtual ssize_t read(void* buf, int32_t count, uint64_t offset) = 0;
  virtual uint64_t length() = 0;
};

#endif
