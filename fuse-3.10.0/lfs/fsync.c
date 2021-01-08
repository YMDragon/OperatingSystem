#include "fsync.h"

int o_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "FSYNC, %s%s, %d, %p\n", prefix, path, isdatasync, fi);
    lfs_writeback();
    //pthread_mutex_unlock(&mutex);
    return 0;
}
