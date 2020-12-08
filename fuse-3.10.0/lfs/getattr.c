#include "getattr.h"

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

int o_getattr(const char *path, struct stat *sbuf, struct fuse_file_info *fi)
{
    logger(DEBUG, "GETATTR, %s%s, %p, %p\n", prefix, path, sbuf, fi);
    options.filename = strdup("hello");
    options.contents = strdup("Hello World!\n");
    (void)fi;
    int res = 0;

    memset(sbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0)
    {
        sbuf->st_mode = S_IFDIR | 0755;
        sbuf->st_nlink = 2;
    }
    else if (strcmp(path + 1, options.filename) == 0)
    {
        sbuf->st_mode = S_IFREG | 0444;
        sbuf->st_nlink = 1;
        sbuf->st_size = strlen(options.contents);
    }
    else
        res = -ENOENT;

    return res;
}
