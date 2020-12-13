#include "chown.h"

int o_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi)
{
    logger(DEBUG, "CHOWN, %s%s, %d, %d, %p\n", prefix, path, uid, gid, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    if (is_owner(fileId) == 0)
        return -EACCES;
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_uid = uid;
    st.st_gid = gid;
    lfs_change(fileId, st);
    return 0;
}
