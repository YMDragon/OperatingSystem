#include "release.h"

int o_release(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "RELEASE, %s%s, %p\n", prefix, path, fi);
  logger(ERROR, "UNIMPLEMENTED: release\n");
  return -1;
}
