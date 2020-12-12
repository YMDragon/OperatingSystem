#include "opendir.h"

int o_opendir(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "OPENDIR, %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    fi->fh = fileId;
    return 0;
}
