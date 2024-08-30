/**
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：archive
 *  Author：Elysia
 *  Date: 24-8-28
 *  Modify Record:
 */
#ifndef INSTALLER_SRC_ARCHIVE_ARCHIVE_H_
#define INSTALLER_SRC_ARCHIVE_ARCHIVE_H_

#include <archive.h>
#include <archive_entry.h>

#include <memory>
#include <string>
#include <vector>
#include <mutex>

extern "C" {
extern int verbose;
static void errmsg(const char *);
static void extract(const char *filename, int do_extract, int flags);
static void fail(const char *, const char *, int);
static int copy_data(struct archive *, struct archive *);
static void msg(const char *);
static void usage(void);
static void warn(const char *, const char *);
}

class Archiver {
 public:
  explicit Archiver(std::string  path);

  ~Archiver();

 private:

  void m_getFilesInArchive();

  void m_openArchive();

  struct archive *m_archive;

  std::string m_filePath;

  std::vector<std::string> m_fileList;

  std::mutex m_fileLock;

  bool m_isFileOpened = false;
};

#endif  // INSTALLER_SRC_ARCHIVE_ARCHIVE_H_
