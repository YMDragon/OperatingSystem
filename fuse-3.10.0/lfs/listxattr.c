#include "listxattr.h"

int o_listxattr(const char *path, char *list, size_t size)
{
    logger(DEBUG, "LISTXATTR, %s%s, %s, %d\n", prefix, path, list, size);
    logger(ERROR, "UNIMPLEMENTED: listxattr\n");
    return -1;
}
