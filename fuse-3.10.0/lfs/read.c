#include "read.h"

int o_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    logger(DEBUG, "READ %s%s, %#X, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    (void)fi;
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    lfs_read(fileId, buf, offset, size);
    return 0;
}
