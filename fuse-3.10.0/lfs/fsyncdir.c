#include "fsyncdir.h"

int o_fsyncdir(const char *path, int isdatasync, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "FSYNCDIR, %s%s, %d, %p\n", prefix, path, isdatasync, fi);
    lfs_writeback();
    //pthread_mutex_unlock(&mutex);
    return 0;
}
