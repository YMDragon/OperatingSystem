#include "read.h"

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
} options1;

int o_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    logger(DEBUG, "READ, %s%s, %s, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    options1.filename = strdup("hello");
    options1.contents = strdup("Hello World!\n");
    size_t len;
    (void)fi;
    if (strcmp(path + 1, options1.filename) != 0)
        return -ENOENT;

    len = strlen(options1.contents);
    if (offset < len)
    {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, options1.contents + offset, size);
    }
    else
        size = 0;

    return size;
}
