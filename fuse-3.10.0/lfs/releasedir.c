#include "releasedir.h"

int o_releasedir(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "RELEASEDIR %s%s, %p\n", prefix, path, fi);
  logger(ERROR, "UNIMPLEMENTED: releasedir\n");
  return -1;
}
