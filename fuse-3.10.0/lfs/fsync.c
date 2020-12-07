#include "fsync.h"

int o_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
  logger(DEBUG, "FSYNC, %s\n", path);
  return -1;
}
