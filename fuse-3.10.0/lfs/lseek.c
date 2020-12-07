#include "lseek.h"

off_t o_lseek(const char *path, off_t off, int whence, struct fuse_file_info *fi)
{
    logger(DEBUG, "LSEEK, %s, %d, %d\n", path, off, whence);
    logger(ERROR, "UNIMPLEMENTED: lseek\n");
    return -1;
}
