#include "utimens.h"

int o_utimens(const char *path, const struct timespec ts[2], struct fuse_file_info *fi)
{
    logger(DEBUG, "UTIMES, %s%s, %p, %p\n", prefix, path, ts, fi);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return fileId;
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_atim = ts[0];
    st.st_mtim = ts[1];
    lfs_change(fileId, st);
    return 0;
}
