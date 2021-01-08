#include "open.h"

int o_open(const char *path, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "OPEN, %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    if (fi->flags & O_TRUNC)
        lfs_truncate(fileId, 0);
    pthread_mutex_unlock(&mutex[fileId]);
    fi->fh = fileId;
    //pthread_mutex_unlock(&mutex);
    return 0;
}
