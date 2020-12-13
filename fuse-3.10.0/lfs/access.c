#include "access.h"

int o_access(const char *path, int mode)
{
    logger(DEBUG, "ACCESS, %s%s, %d\n", prefix, path, mode);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    if (has_permission(fileId, mode) == 0)
        return -EACCES;
    return 0;
}
