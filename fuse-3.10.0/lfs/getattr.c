#include "getattr.h"

int o_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    logger(DEBUG, "GETATTR, %s%s, %p, %p\n", prefix, path, stbuf, fi);
    (void)fi;
    logger(DEBUG, "%s\n", path);
    int fileId = get_fileId(path);
    logger(DEBUG, "fileId %d\n", fileId);
    if (fileId < 0)
        return fileId;
    if (lfs_metadata(fileId, stbuf) < 0)
        return -ENOENT;
    return 0;
}
