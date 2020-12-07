#include "releasedir.h"

int o_releasedir(const char *path, struct fuse_file_info *fi)
{
  logger(DEBUG, "RELEASEDIR %s\n", path);
  return -1;
}
