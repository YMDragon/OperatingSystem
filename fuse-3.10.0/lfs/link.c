#include "link.h"

int o_link(const char *from, const char *to)
{
    logger(DEBUG, "LINK, %s%s, %s%s\n", prefix, from, prefix, to);
    int fileId = get_fileId(from);
    if (fileId < 0)
        return -fileId;
    int pos, size = strlen(to);
    int parentfileId = get_parent_fileId(to, &pos);
    add_entry(parentfileId, to + pos, size - pos, fileId);
    struct stat st;
    lfs_metadata(fileId, &st);
    st.st_nlink++;
    lfs_change(fileId, st);
    return 0;
}
