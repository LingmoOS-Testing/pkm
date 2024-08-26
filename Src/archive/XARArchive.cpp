#include "archive/XARArchive.h"

#include <zlib.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

XARArchive::XARArchive(const std::string& path)
    : m_toc(nullptr) {
  struct archive_entry *entry;
  int r;

  a = archive_read_new();
  archive_read_support_format_xar(a);
  r = archive_read_open_filename(a, path.c_str(), 10240);
  if (r != ARCHIVE_OK)
    throw std::runtime_error("Cannot read the XAR archive! Is it damaged?");

  if (archive_format(a) != ARCHIVE_FORMAT_XAR)
    throw std::runtime_error("Not a XAR file");
}

XARArchive::~XARArchive() {
  xmlXPathFreeContext(m_context);
  xmlFreeDoc(m_toc);
}

static std::vector<std::string>& split(const std::string& s, char delim,
                                       std::vector<std::string>& elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) elems.push_back(item);

  return elems;
}

static std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

std::string XARArchive::xpathForPath(const std::string& path) {
  std::vector<std::string> parts = split(path, '/');
  std::stringstream ss;

  if (parts.empty()) return std::string();

  ss << "/xar/toc";

  for (std::string p : parts) {
    if (p.empty()) continue;

    // libxml2 only supports XPath 1.0
    std::transform(p.begin(), p.end(), p.begin(), ::tolower);
    // TODO: escape file name
    ss << "/file[translate(name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', "
          "'abcdefghijklmnopqrstuvwxyz')='"
       << p << "']";
  }
  ss << "/data";

  return ss.str();
}

Reader* XARArchive::openFile(const std::string& path) {
  xmlXPathObjectPtr xpathObj;
  Reader* retval = nullptr;
  std::string xpath;

  xpath = xpathForPath(path);

  if (xpath.empty()) return nullptr;

  m_context->node = nullptr;

  xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), m_context);

  // nodesetval：存储结果列表，如果为NULL，表示无结果
  if (xpathObj->nodesetval) {
    retval = openFile(xpathObj->nodesetval);
  }

  xmlXPathFreeObject(xpathObj);
  return retval;
}

bool XARArchive::containsFile(const std::string& path) {
  std::string xpath;
  xmlXPathObjectPtr xpathObj;
  bool rv = false;

  xpath = xpathForPath(path);
  if (xpath.empty()) return false;

  m_context->node = nullptr;

  xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath.c_str(), m_context);
  if (xpathObj->nodesetval) rv = true;

  xmlXPathFreeObject(xpathObj);
  return rv;
}

Reader* XARArchive::openFile(xmlNodeSetPtr nodes) {
  xmlXPathObjectPtr xpathObj;
  Reader* retval = nullptr;
  int64_t length, offset, size;

  if (!nodes->nodeNr) return nullptr;

  m_context->node = nodes->nodeTab[0];
  xpathObj = xmlXPathEvalExpression((const xmlChar*)"string(encoding/@style)",
                                    m_context);

  if (!xpathObj->stringval) {
    xmlXPathFreeObject(xpathObj);
    throw std::runtime_error("Missing encoding for file!");
  }

  length = extractXMLNumber(m_context, "string(length)");
  offset = extractXMLNumber(m_context, "string(offset)");
  size = extractXMLNumber(m_context, "string(size)");

  retval =
      ArchivedFileReader::create((const char*)xpathObj->stringval, m_reader,
                                 length, m_heapStart + offset, size);

  xmlXPathFreeObject(xpathObj);
  return retval;
}

int64_t XARArchive::extractXMLNumber(xmlXPathContextPtr context,
                                     const char* query) {
  xmlXPathObjectPtr xpathObj;
  int64_t rv = -1;

  xpathObj = xmlXPathEvalExpression((const xmlChar*)query, context);
  if (xpathObj && xpathObj->stringval)
    rv = std::stoll((const char*)xpathObj->stringval);

  xmlXPathFreeObject(xpathObj);
  return rv;
}

std::vector<std::string> XARArchive::listFiles() {
  std::vector<std::string> rv;
  return rv;
}

void XARArchive::decompressTOC(const char* in, size_t inLength, char* out,
                               size_t outLength) {
  z_stream strm;
  int ret;

  memset(&strm, 0, sizeof(strm));

  ret = inflateInit(&strm);
  if (ret != Z_OK) throw std::runtime_error("deflateInit failed");

  strm.next_in = (Bytef*)in;
  strm.avail_in = inLength;
  strm.next_out = (Bytef*)out;
  strm.avail_out = outLength;

  while (true) {
    ret = inflate(&strm, Z_NO_FLUSH);
    if (ret < 0)
      throw std::runtime_error("deflate error");
    else if (ret == Z_STREAM_END)
      break;
  }

  inflateEnd(&strm);
}

void XARArchive::loadTOC(const char* in, size_t inLength) {
  m_toc = xmlParseMemory(in, inLength);
  if (!m_toc) throw std::runtime_error("Invalid XML data");
  m_context = xmlXPathNewContext(m_toc);
  // std::cout << std::string(in, inLength) << std::endl;
}
