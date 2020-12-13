#include "directory.h"

int add_entry(int fileId, const char *name, int size, int childfileId)
{
    struct stat st;
    struct DirectoryEntry de;
    memset(&de, 0, sizeof(struct DirectoryEntry));
    memcpy(de.name, name, size);
    de.fileId = childfileId;
    lfs_metadata(fileId, &st);
    lfs_write(fileId, (char *)&de, st.st_size, sizeof(struct DirectoryEntry));
    return 0;
}

int create_dir(const char *name, int size, struct stat st, int parentfileId)
{
    st.st_nlink = 2;
    int fileId = lfs_create(st);
    if (parentfileId > 0)
    {
        struct stat parentst;
        lfs_metadata(parentfileId, &parentst);
        parentst.st_nlink++;
        lfs_change(parentfileId, parentst);
        add_entry(parentfileId, name, size, fileId);
    }
    else
        parentfileId = fileId;
    //add_entry(fileId, "..", 2, parentfileId);
    //add_entry(fileId, ".", 1, fileId);
    //lfs_change(fileId, st);
    return fileId;
}

int get_fileId(const char *path)
{
    if (path[0] != '/')
        return -ENOENT;
    int size = strlen(path);
    int fileId = rootId;
    struct stat st;
    int l, r, i, found;
    char *buf = malloc(sizeof(struct DirectoryEntry));
    struct DirectoryEntry *de;
    for (l = 1; l < size; l = r + 1)
    {
        for (r = l; r < size; r++)
            if (path[r] == '/')
                break;
        //logger(DEBUG, "%s %d %d\n", path, l, r);
        if (lfs_metadata(fileId, &st) < 0)
            return -ENOENT;
        if ((st.st_mode & S_IFDIR) == 0)
            return -ENOTDIR;
        if (has_permission(fileId, R_OK) == 0)
            return -EACCES;
        found = 0;
        for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
        {
            lfs_read(fileId, buf, i, sizeof(struct DirectoryEntry));
            de = (struct DirectoryEntry *)buf;
            if (is_match(de, path + l, r - l))
            {
                fileId = de->fileId;
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            free(buf);
            return -ENOENT;
        }
    }
    free(buf);
    return fileId;
}

int get_parent_fileId(const char *path, int *pos)
{
    if (path[0] != '/')
        return -ENOENT;
    int size = strlen(path);
    int fileId = rootId;
    struct stat st;
    int l, r, i, found;
    char *buf = malloc(sizeof(struct DirectoryEntry));
    struct DirectoryEntry *de;
    for (l = 1; l < size; l = r + 1)
    {
        for (r = l; r < size; r++)
            if (path[r] == '/')
                break;
        if (lfs_metadata(fileId, &st) < 0)
            return -ENOENT;
        if ((st.st_mode & S_IFDIR) == 0)
            return -ENOTDIR;
        if (has_permission(fileId, R_OK) == 0)
            return -EACCES;
        if (r >= size - 1)
        {
            *pos = l;
            return fileId;
        }
        found = 0;
        for (i = 0; i < st.st_size; i += sizeof(struct DirectoryEntry))
        {
            lfs_read(fileId, buf, i, sizeof(struct DirectoryEntry));
            de = (struct DirectoryEntry *)buf;
            if (is_match(de, path + l, r - l))
            {
                fileId = de->fileId;
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            free(buf);
            return -ENOENT;
        }
    }
    free(buf);
    return fileId;
}

int is_match(struct DirectoryEntry *de, const char *name, int size)
{
    int i;
    if (de->name[size] != 0)
        return 0;
    for (i = 0; i < size; i++)
        if (de->name[i] != name[i])
            return 0;
    return 1;
}