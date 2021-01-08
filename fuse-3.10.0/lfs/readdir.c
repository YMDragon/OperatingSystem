#include "readdir.h"

int o_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "READDIR %s%s, %#X, %d, %d, %p, %d\n", prefix, path, buf, filler, offset, fi, flags);
    (void)offset;
    (void)fi;
    (void)flags;

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
    struct stat st, childst;
    char *buffer = malloc(sizeof(struct DirectoryEntry));
    struct DirectoryEntry *de;
    int i;
    if (lfs_metadata(fileId, &st) < 0)
    {
        pthread_mutex_unlock(&mutex[fileId]);
        //pthread_mutex_unlock(&mutex);
        return -ENOENT;
    }
    //logger(DEBUG, "%d %d %d\n", fileId, st.st_size, sizeof(struct DirectoryEntry));
    filler(buf, "..", NULL, 0, 0);
    filler(buf, ".", NULL, 0, 0);
    for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
    {
        lfs_read(fileId, buffer, i, sizeof(struct DirectoryEntry));
        de = (struct DirectoryEntry *)buffer;
        if (lfs_metadata(de->fileId, &childst) < 0)
        {
            free(buffer);
            pthread_mutex_unlock(&mutex[fileId]);
            //pthread_mutex_unlock(&mutex);
            return -ENOENT;
        }
        //logger(DEBUG, "%s %d\n", de->name, de->fileId);
        //filler(buf, de->name, &childst, 0, 0);
        filler(buf, de->name, NULL, 0, 0);
    }
    free(buffer);
    pthread_mutex_unlock(&mutex[fileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
