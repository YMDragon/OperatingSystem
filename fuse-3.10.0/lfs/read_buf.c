#include "read_buf.h"

int o_read_buf(const char *path, struct fuse_bufvec **buf, size_t size, off_t off, struct fuse_file_info *fi)
{
    logger(DEBUG, "READ_BUF, %s%s, %p, %d, %d, %p\n", prefix, path, buf, size, off, fi);
    logger(ERROR, "UNIMPLEMENTED: read_buf\n");
    return -1;
}
