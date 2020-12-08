#include "open.h"

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

int o_open(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "OPEN, %s%s, %p\n", prefix, path, fi);
    options.filename = strdup("hello");
    options.contents = strdup("Hello World!\n");
    if (strcmp(path + 1, options.filename) != 0)
        return -ENOENT;

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return -EACCES;

    return 0;
}
