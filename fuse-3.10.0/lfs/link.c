#include "link.h"

int o_link(const char *from, const char *to)
{
    logger(DEBUG, "LINK, %s%s, %s%s\n", prefix, from, prefix, to);
    logger(ERROR, "UNIMPLEMENTED: link, from: %s, to: %s\n", from, to);
    return -1;
}
