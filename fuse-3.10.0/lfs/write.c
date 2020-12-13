#include "write.h"

int o_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    logger(DEBUG, "WRITE, %s%s, %s, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    if ((fi->flags & O_ACCMODE) != O_WRONLY && (fi->flags & O_ACCMODE) != O_RDWR)
        return -1;
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    if (has_permission(fileId, W_OK) == 0)
        return -EACCES;
    return lfs_write(fileId, buf, offset, size);
}
