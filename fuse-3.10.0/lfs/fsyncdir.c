#include "fsyncdir.h"

int o_fsyncdir(const char *path, int isdatasync, struct fuse_file_info *fi)
{
  logger(DEBUG, "FSYNCDIR, %s\n", path);
  return -1;
}
