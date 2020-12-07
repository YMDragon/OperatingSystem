#include "statfs.h"

int o_statfs(const char *path, struct statvfs *stbuf)
{
  logger(DEBUG, "STATFS, %s\n", path);
  return -1;
}
