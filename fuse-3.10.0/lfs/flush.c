#include "flush.h"

int o_flush(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "FLUSH, %s\n", path);
  return -1;
}
