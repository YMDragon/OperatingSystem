#include "rename.h"

int o_rename(const char *from, const char *to, unsigned int flags)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "RENAME, %s%s, %s%s, %d\n", prefix, from, prefix, to, flags);
    int fileId = get_fileId(from);
    if (fileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return -fileId;
    }
    pthread_mutex_lock(&mutex[fileId]);
    int pos, size = strlen(from);
    int parentfileId;
    parentfileId = get_parent_fileId(to, &pos);
    pthread_mutex_lock(&mutex[parentfileId]);
    if (has_permission(parentfileId, W_OK) == 0)
    {
        pthread_mutex_unlock(&mutex[parentfileId]);
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    pthread_mutex_unlock(&mutex[parentfileId]);
    parentfileId = get_parent_fileId(from, &pos);
    pthread_mutex_lock(&mutex[parentfileId]);
    if (has_permission(parentfileId, W_OK) == 0)
    {
        pthread_mutex_unlock(&mutex[parentfileId]);
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -EACCES;
    }
    struct stat st;
    char *buf = malloc(sizeof(struct DirectoryEntry));
    struct DirectoryEntry *de;
    lfs_metadata(parentfileId, &st);
    int i;
    for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
    {
        lfs_read(parentfileId, buf, i, sizeof(struct DirectoryEntry));
        de = (struct DirectoryEntry *)buf;
        if (is_match(de, from + pos, size - pos))
        {
            lfs_read(parentfileId, buf, st.st_size - sizeof(struct DirectoryEntry), sizeof(struct DirectoryEntry));
            lfs_write(parentfileId, buf, i, sizeof(struct DirectoryEntry));
            break;
        }
    }
    lfs_truncate(parentfileId, st.st_size - sizeof(struct DirectoryEntry));
    pthread_mutex_unlock(&mutex[parentfileId]);
    size = strlen(to);
    parentfileId = get_parent_fileId(to, &pos);
    pthread_mutex_lock(&mutex[parentfileId]);
    lfs_metadata(parentfileId, &st);
    printf("%ld\n", st.st_size);
    for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
    {
        lfs_read(parentfileId, buf, i, sizeof(struct DirectoryEntry));
        de = (struct DirectoryEntry *)buf;
        if (is_match(de, to + pos, size - pos))
        {
            de->fileId = fileId;
            lfs_write(parentfileId, buf, i, sizeof(struct DirectoryEntry));
            free(buf);
            pthread_mutex_unlock(&mutex[parentfileId]);
            pthread_mutex_unlock(&mutex[fileId]);
            //pthread_mutex_unlock(&mutex);
            return 0;
        }
    }
    memset(de, 0, sizeof(struct DirectoryEntry));
    memcpy(de->name, to + pos, size);
    de->fileId = fileId;
    lfs_write(parentfileId, buf, st.st_size, sizeof(struct DirectoryEntry));
    free(buf);
    pthread_mutex_unlock(&mutex[parentfileId]);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
