#include "bmap.h"

int o_bmap(const char *path, size_t blocksize, uint64_t *idx)
{
    logger(DEBUG, "BMAP, %s, %d, %lld\n", path, blocksize, idx);
    logger(ERROR, "UNIMPLEMENTED: bmap\n");
    return -1;
}
