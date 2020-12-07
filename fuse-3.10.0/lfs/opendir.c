#include "opendir.h"

int o_opendir(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "OPENDIR, %s\n", path);
  logger(ERROR, "UNIMPLEMENTED: opendir, path: %s\n", path);
  return -1;
}
