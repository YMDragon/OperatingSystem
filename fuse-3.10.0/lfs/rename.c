#include "rename.h"

int o_rename(const char *from, const char *to)
{
    logger(DEBUG, "RENAME, %s, %s\n", from, to);
    logger(ERROR, "UNIMPLEMENTED: rename, from: %s, to: %s\n", from, to);
    return -1;
}
