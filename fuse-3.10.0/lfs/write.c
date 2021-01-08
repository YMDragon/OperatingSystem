#include "write.h"

int o_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "WRITE, %s%s, %#X, %d, %d, %p\n", prefix, path, buf, size, offset, fi);
    if ((fi->flags & O_ACCMODE) != O_WRONLY && (fi->flags & O_ACCMODE) != O_RDWR)
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
    if (has_permission(fileId, W_OK) == 0)
    {
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    size = lfs_write(fileId, buf, offset, size);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return size;
}
