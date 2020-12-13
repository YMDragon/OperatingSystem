#include "rmdir.h"

int o_rmdir(const char *path)
{
    logger(DEBUG, "RMDIR, %s%s\n", prefix, path);
    int fileId = get_fileId(path);
    if (fileId < 0)
        return -fileId;
    int pos, size = strlen(path);
    int parentfileId = get_parent_fileId(path, &pos);
    if (has_permission(parentfileId, W_OK) == 0)
        return -EACCES;
    struct stat st;
    char *buf = malloc(sizeof(struct DirectoryEntry));
    struct DirectoryEntry *de;
    lfs_metadata(parentfileId, &st);
    int i;
    for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
    {
        lfs_read(parentfileId, buf, i, sizeof(struct DirectoryEntry));
        de = (struct DirectoryEntry *)buf;
        if (is_match(de, path + pos, size - pos))
        {
            lfs_read(parentfileId, buf, st.st_size - sizeof(struct DirectoryEntry), sizeof(struct DirectoryEntry));
            lfs_write(parentfileId, buf, i, sizeof(struct DirectoryEntry));
            break;
        }
    }
    lfs_truncate(parentfileId, st.st_size - sizeof(struct DirectoryEntry));
    lfs_metadata(fileId, &st);
    st.st_nlink--;
    lfs_change(fileId, st);
    if (st.st_nlink == 1)
        lfs_remove(fileId);
    free(buf);
    return 0;
}
