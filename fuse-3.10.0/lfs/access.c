#include "access.h"

int o_access(const char *path, int mode)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "ACCESS, %s%s, %d\n", prefix, path, mode);
    int fileId = get_fileId(path);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    if (has_permission(fileId, mode) == 0)
    {
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
