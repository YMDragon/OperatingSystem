#include "mkdir.h"

int o_mkdir(const char *path, mode_t mode)
{
    logger(DEBUG, "MKDIR, %s%s, %o\n", prefix, path, mode);
    int pos, size = strlen(path);
    int parentfileId = get_parent_fileId(path, &pos);
    if (parentfileId < 0)
    {
        //logger(DEBUG, "wrong!!!!!!!!!!!\n");
        return -parentfileId;
    }
    if (has_permission(parentfileId, W_OK) == 0)
    {
        //logger(DEBUG, "wrong!!!!!!!!!!!\n");
        return -EACCES;
    }
    struct stat st;
    struct fuse_context *fc = fuse_get_context();
    st.st_mode = S_IFDIR | mode;
    st.st_uid = fc->uid;
    st.st_gid = fc->gid;
    create_dir(path + pos, size - pos, st, parentfileId);
    return 0;
}
