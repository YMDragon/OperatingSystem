#include "permission.h"

int has_permission(struct stat st, int mode)
{
    struct fuse_context *fc = fuse_get_context();
    if (st.st_uid == fc->uid)
        return ((st.st_mode >> 6) & 7 & mode) == mode;
    if (st.st_gid == fc->gid)
        return ((st.st_mode >> 3) & 7 & mode) == mode;
    return (st.st_mode & 7 & mode) == mode;
}