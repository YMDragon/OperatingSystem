#include "release.h"

int o_release(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "RELEASE, %s\n", path);
  return -1;
}
