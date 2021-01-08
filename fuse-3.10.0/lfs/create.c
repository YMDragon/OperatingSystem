#include "create.h"

int o_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "CREATE, %s%s, %d, %p\n", prefix, path, mode, fi);
    int pos, size = strlen(path);
    int parentfileId = get_parent_fileId(path, &pos);
    if (parentfileId < 0)
    {
        //pthread_mutex_unlock(&mutex);
        return -parentfileId;
    }
    pthread_mutex_lock(&mutex[parentfileId]);
    struct stat st;
    struct fuse_context *fc = fuse_get_context();
    st.st_mode = S_IFREG | mode;
    st.st_uid = fc->uid;
    st.st_gid = fc->gid;
    st.st_nlink = 1;
    int fileId = lfs_create(st);
    pthread_mutex_init(&mutex[fileId], NULL);
    add_entry(parentfileId, path + pos, size - pos, fileId);
    fi->fh = fileId;
    pthread_mutex_unlock(&mutex[parentfileId]);
    //pthread_mutex_unlock(&mutex);
    return 0;
}
