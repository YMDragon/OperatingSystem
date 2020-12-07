#include "./getattr.h"         /* o_getattr */
#include "./readlink.h"        /* o_readlink */
#include "./mknod.h"           /* mknod */
#include "./mkdir.h"           /* o_mkdir */
#include "./unlink.h"          /* o_unlink */
#include "./rmdir.h"           /* o_rmdir */
#include "./symlink.h"         /* symlink */
#include "./rename.h"          /* o_rename */
#include "./link.h"            /* link */
#include "./chmod.h"           /* o_chmod */
#include "./chown.h"           /* o_chown */
#include "./truncate.h"        /* o_truncate */
#include "./open.h"            /* o_open */
#include "./read.h"            /* o_read */
#include "./write.h"           /* o_write */
#include "./statfs.h"          /* o_statfs */
#include "./flush.h"           /* o_flush */
#include "./release.h"         /* o_release */
#include "./fsync.h"           /* o_fsync */
#include "./setxattr.h"        /* setxattr */
#include "./getxattr.h"        /* getxattr */
#include "./listxattr.h"       /* listxattr */
#include "./removexattr.h"     /* removexattr */
#include "./opendir.h"         /* o_opendir */
#include "./readdir.h"         /* o_readdir */
#include "./releasedir.h"      /* o_releasedir */
#include "./fsyncdir.h"        /* o_fsyncdir */
#include "./init.h"            /* o_init */
#include "./destroy.h"         /* o_destroy */
#include "./access.h"          /* o_access */
#include "./create.h"          /* create */
#include "./lock.h"            /* o_lock */
#include "./utimens.h"         /* o_utimens */
#include "./bmap.h"            /* bmap */
#include "./ioctl.h"           /* ioctl */
#include "./poll.h"            /* poll */
#include "./write_buf.h"       /* write_buf */
#include "./read_buf.h"        /* read_buf */
#include "./flock.h"           /* flock */
#include "./fallocate.h"       /* fallocate */
#include "./copy_file_range.h" /* copy_file_range */
#include "./lseek.h"           /* lseek */

#include <fuse.h> /* fuse_main */

#include "./logger.h" /* set_log_level set_log_output logger */

static struct fuse_operations ops = {
    .getattr = o_getattr,
    .readlink = o_readlink,
    .mknod = o_mknod,
    .mkdir = o_mkdir,
    .unlink = o_unlink,
    .rmdir = o_rmdir,
    .symlink = o_symlink,
    .rename = o_rename,
    .link = o_link,
    .chmod = o_chmod,
    .chown = o_chown,
    .truncate = o_truncate,
    .open = o_open,
    .read = o_read,
    .write = o_write,
    .statfs = o_statfs,
    .flush = o_flush,
    .release = o_release,
    .fsync = o_fsync,
    .setxattr = o_setxattr,
    .getxattr = o_getxattr,
    .listxattr = o_listxattr,
    .removexattr = o_removexattr,
    .opendir = o_opendir,
    .readdir = o_readdir,
    .releasedir = o_releasedir,
    .fsyncdir = o_fsyncdir,
    .init = o_init,
    .destroy = o_destroy,
    .access = o_access,
    .create = o_create,
    .lock = o_lock,
    .utimens = o_utimens,
    .bmap = o_bmap,
    .ioctl = o_ioctl,
    .poll = o_poll,
    .write_buf = o_write_buf,
    .read_buf = o_read_buf,
    .flock = o_flock,
    .fallocate = o_fallocate,
    .copy_file_range = o_copy_file_range,
    .lseek = o_lseek};

int main(int argc, char **argv)
{
    set_log_level(DEBUG);
    set_log_output(stdout);

    return fuse_main(argc, argv, &ops, NULL);
}
