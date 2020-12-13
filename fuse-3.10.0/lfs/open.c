#include "open.h"

int o_open(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "OPEN, %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    if (fi->flags & O_TRUNC)
        lfs_truncate(fileId, 0);
    fi->fh = fileId;
    return 0;
}
