#include "truncate.h"

int o_truncate(const char *path, off_t size, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "TRUNCATE, %s%s, %d, %p\n", prefix, path, size, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    lfs_truncate(fileId, size);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
