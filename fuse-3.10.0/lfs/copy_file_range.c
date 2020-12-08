#include "copy_file_range.h"

ssize_t o_copy_file_range(const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out, struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags)
{
    logger(DEBUG, "COPY_FILE_RANGE, %s%s, %p, %d, %s%s, %p, %d, %d, %d\n", prefix, path_in, fi_in, offset_in, prefix, path_out, fi_out, offset_out, size, flags);
    logger(ERROR, "UNIMPLEMENTED: copy_file_range\n");
    return -1;
}
