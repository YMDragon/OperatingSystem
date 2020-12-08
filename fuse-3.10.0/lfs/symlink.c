#include "symlink.h"

int o_symlink(const char *from, const char *to)
{
    logger(DEBUG, "SYMLINK, %s%s, %s%s\n", prefix, from, prefix, to);
    logger(ERROR, "UNIMPLEMENTED: symlink, from: %s, to: %s\n", from, to);
    return -1;
}
