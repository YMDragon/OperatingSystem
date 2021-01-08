#include "utimens.h"

int o_utimens(const char *path, const struct timespec ts[2], struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "UTIMES, %s%s, %p, %p\n", prefix, path, ts, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_atim = ts[0];
    st.st_mtim = ts[1];
    lfs_change(fileId, st);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
