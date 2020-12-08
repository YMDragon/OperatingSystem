#include "readdir.h"

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

static struct options
{
  const char *filename;
  const char *contents;
  int show_help;
} options;

int o_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
  logger(DEBUG, "READDIR %s%s, %#X, %d, %d, %p, %d\n", prefix, path, buf, filler, offset, fi, flags);
  options.filename = strdup("hello");
  options.contents = strdup("Hello World!\n");
  (void)offset;
  (void)fi;
  (void)flags;

  if (strcmp(path, "/") != 0)
    return -ENOENT;

  filler(buf, ".", NULL, 0, 0);
  filler(buf, "..", NULL, 0, 0);
  filler(buf, options.filename, NULL, 0, 0);

  return 0;
}
