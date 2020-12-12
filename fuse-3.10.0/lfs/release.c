#include "release.h"

int o_release(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "RELEASE, %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    fi->fh = 0;
    return -1;
}
