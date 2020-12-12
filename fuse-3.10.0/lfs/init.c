#include "init.h"

#include "stdio.h"

void *o_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
    logger(DEBUG, "INIT %p, %p\n", conn, cfg);
    if (file_number() == 0)
    {
        struct stat st;
        struct fuse_context *fc = fuse_get_context();
        st.st_mode = S_IFDIR | 0755;
        st.st_uid = fc->uid;
        st.st_gid = fc->gid;
        create_dir("/", 1, st, -1);
    }
    rootId = 1;
    return NULL;
}
