#include "rename.h"

int o_rename(const char *from, const char *to, unsigned int flags)
{
    logger(DEBUG, "RENAME, %s, %s, %d\n", from, to, flags);
    logger(ERROR, "UNIMPLEMENTED: rename, from: %s, to: %s\n", from, to);
    return -1;
}
