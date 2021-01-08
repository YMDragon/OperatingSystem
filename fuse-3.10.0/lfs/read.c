#include "read.h"

int o_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "READ %s%s, %#X, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    if ((fi->flags & O_ACCMODE) != O_RDONLY && (fi->flags & O_ACCMODE) != O_RDWR)
    {
        //pthread_mutex_unlock(&mutex);
        return -1;
    }
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    if (has_permission(fileId, R_OK) == 0)
    {
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    size = lfs_read(fileId, buf, offset, size);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return size;
}
