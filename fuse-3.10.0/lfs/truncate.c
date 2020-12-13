#include "truncate.h"

int o_truncate(const char *path, off_t size, struct fuse_file_info *fi)
{
    logger(DEBUG, "TRUNCATE, %s%s, %d, %p\n", prefix, path, size, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    return lfs_truncate(fileId, size);
}
