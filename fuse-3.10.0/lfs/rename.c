#include "rename.h"

int o_rename(const char *from, const char *to, unsigned int flags)
{
    logger(DEBUG, "RENAME, %s%s, %s%s, %d\n", prefix, from, prefix, to, flags);
    int fileId = get_fileId(from);
    if (fileId < 0)
        return -fileId;
    int pos, size = strlen(from);
    int parentfileId;
    parentfileId = get_parent_fileId(to, &pos);
    if (has_permission(parentfileId, W_OK) == 0)
        return -EACCES;
    parentfileId = get_parent_fileId(from, &pos);
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
        if (is_match(de, from + pos, size - pos))
        {
            lfs_read(parentfileId, buf, st.st_size - sizeof(struct DirectoryEntry), sizeof(struct DirectoryEntry));
            lfs_write(parentfileId, buf, i, sizeof(struct DirectoryEntry));
            break;
        }
    }
    lfs_truncate(parentfileId, st.st_size - sizeof(struct DirectoryEntry));
    size = strlen(to);
    parentfileId = get_parent_fileId(to, &pos);
    lfs_metadata(parentfileId, &st);
    for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
    {
        lfs_read(parentfileId, buf, i, sizeof(struct DirectoryEntry));
        de = (struct DirectoryEntry *)buf;
        if (is_match(de, to + pos, size - pos))
        {
            de->fileId = fileId;
            lfs_write(parentfileId, buf, i, sizeof(struct DirectoryEntry));
            free(buf);
            return 0;
        }
    }
    memset(de, 0, sizeof(struct DirectoryEntry));
    memcpy(de->name, to + pos, size);
    de->fileId = fileId;
    lfs_write(parentfileId, buf, st.st_size, sizeof(struct DirectoryEntry));
    free(buf);
    return 0;
}
