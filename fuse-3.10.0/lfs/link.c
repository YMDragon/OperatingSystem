#include "link.h"

int o_link(const char *from, const char *to)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "LINK, %s%s, %s%s\n", prefix, from, prefix, to);
    int fileId = get_fileId(from);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return -fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    int pos, size = strlen(to);
    int parentfileId = get_parent_fileId(to, &pos);
    pthread_mutex_lock(&mutex[parentfileId]);
    add_entry(parentfileId, to + pos, size - pos, fileId);
    pthread_mutex_unlock(&mutex[parentfileId]);
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_nlink++;
    lfs_change(fileId, st);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
