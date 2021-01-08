#include "opendir.h"

int o_opendir(const char *path, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "OPENDIR, %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    fi->fh = fileId;
    //pthread_mutex_unlock(&mutex);
    return 0;
}
