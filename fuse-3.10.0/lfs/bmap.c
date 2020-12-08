#include "bmap.h"

int o_bmap(const char *path, size_t blocksize, uint64_t *idx)
{
    logger(DEBUG, "BMAP, %s%s, %d, %p\n", prefix, path, blocksize, idx);
    logger(ERROR, "UNIMPLEMENTED: bmap\n");
    return -1;
}
