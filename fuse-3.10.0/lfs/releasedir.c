#include "releasedir.h"

int o_releasedir(const char *path, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "RELEASEDIR %s%s, %p\n", prefix, path, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    fi->fh = 0;
    //pthread_mutex_unlock(&mutex);
    return 0;
}
