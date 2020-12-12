#include "write.h"

int o_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    logger(DEBUG, "WRITE, %s%s, %s, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    (void)fi;
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    lfs_read(fileId, buf, offset, size);
    return -1;
}
