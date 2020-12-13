#include "permission.h"

int has_permission(int fileId, int mode)
{
    struct stat st;
    struct fuse_context *fc = fuse_get_context();
    lfs_metadata(fileId, &st);
    if (st.st_uid == fc->uid)
        return ((st.st_mode >> 6) & 7 & mode) == mode;
    if (st.st_gid == fc->gid)
        return ((st.st_mode >> 3) & 7 & mode) == mode;
    return (st.st_mode & 7 & mode) == mode;
}

int is_owner(int fileId)
{
    struct stat st;
    struct fuse_context *fc = fuse_get_context();
    lfs_metadata(fileId, &st);
    return st.st_uid == fc->uid;
}