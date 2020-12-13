#include "releasedir.h"

int o_releasedir(const char *path, struct fuse_file_info *fi)
{
    logger(DEBUG, "RELEASEDIR %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    fi->fh = 0;
    return 0;
}