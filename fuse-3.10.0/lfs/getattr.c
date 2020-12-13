#include "getattr.h"

int o_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
    logger(DEBUG, "GETATTR, %s%s, %p, %p\n", prefix, path, stbuf, fi);
    (void)fi;
    int fileId = get_fileId(path);
    //logger(DEBUG, "%d\n", fileId);
    if (fileId < 0)
        return fileId;
    //logger(DEBUG, "%d\n", lfs_metadata(fileId, stbuf));
    //logger(DEBUG, "size %d\n", stbuf->st_size);
    if (lfs_metadata(fileId, stbuf) < 0)
        return -ENOENT;
    //logger(DEBUG, "inode %d\n", stbuf->st_ino);
    return 0;
}
