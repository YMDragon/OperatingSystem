#include "access.h"

int o_access(const char *path, int mode)
{
    logger(DEBUG, "ACCESS, %s%s, %d\n", prefix, path, mode);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    struct stat st;
    lfs_metadata(fileId, &st);
    if (has_permission(st, mode) == 0)
        return -EACCES;
    return 0;
}
