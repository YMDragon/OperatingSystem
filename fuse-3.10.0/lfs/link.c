#include "link.h"

int o_link(const char *from, const char *to)
{
    logger(DEBUG, "LINK, %s, %s\n", from, to);
    logger(ERROR, "UNIMPLEMENTED: link, from: %s, to: %s\n", from, to);
    return -1;
}
