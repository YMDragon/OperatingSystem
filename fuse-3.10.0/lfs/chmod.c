#include "chmod.h"

int o_chmod(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "CHMOD, %s%s, %d, $p\n", prefix, path, mode, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    if (is_owner(fileId) == 0)
    {
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_mode = ((st.st_mode >> 9) << 9) | mode;
    lfs_change(fileId, st);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
