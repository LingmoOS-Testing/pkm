#ifndef XARARCHIVE_H
#define XARARCHIVE_H

#include <xar.h>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <cstdint>
#include <memory>

#include "archive/RandomAccessDirectory.h"

class XARArchive : public RandomAccessDirectory {
 public:
  XARArchive(const std::string& path);
  virtual ~XARArchive();
  virtual Reader* openFile(const std::string& path) override;
  virtual std::vector<std::string> listFiles() override;
  bool containsFile(const std::string& path);

 private:
  std::string xpathForPath(const std::string& path);
  Reader* openFile(xmlNodeSetPtr nodes);
  static void decompressTOC(const char* in, size_t inLength, char* out,
                            size_t outLength);
  static int64_t extractXMLNumber(xmlXPathContextPtr context,
                                  const char* query);
  void loadTOC(const char* in, size_t inLength);

 private:
  struct archive* a;
  xmlDocPtr m_toc;
  xmlXPathContextPtr m_context;
  uint64_t m_heapStart;
};

#endif
