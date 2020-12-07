#include "write_buf.h"

int o_write_buf(const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi)
{
    logger(DEBUG, "WRITE_BUF, %s, %d\n", path, off);
    logger(ERROR, "UNIMPLEMENTED: write_buf\n");
    return -1;
}
