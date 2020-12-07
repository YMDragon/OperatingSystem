#include "./getattr.h"  /* o_getattr */
#include "./readlink.h" /* o_readlink */
// mknod
#include "./mkdir.h"  /* o_mkdir */
#include "./unlink.h" /* o_unlink */
#include "./rmdir.h"  /* o_rmdir */
// symlink
#include "./rename.h" /* o_rename */
// link
#include "./chmod.h"    /* o_chmod */
#include "./chown.h"    /* o_chown */
#include "./truncate.h" /* o_truncate */
#include "./open.h"     /* o_open */
#include "./read.h"     /* o_read */
#include "./write.h"    /* o_write */
#include "./statfs.h"   /* o_statfs */
#include "./flush.h"    /* o_flush */
#include "./release.h"  /* o_release */
#include "./fsync.h"    /* o_fsync */
// setxattr
// getxattr
// listxattr
// removexattr
#include "./opendir.h"    /* o_opendir */
#include "./readdir.h"    /* o_readdir */
#include "./releasedir.h" /* o_releasedir */
#include "./fsyncdir.h"   /* o_fsyncdir */
#include "./init.h"       /* o_init */
#include "./destroy.h"    /* o_destroy */
#include "./access.h"     /* o_access */
// create
#include "./lock.h"    /* o_lock */
#include "./utimens.h" /* o_utimens */
// bmap
// ioctl
// poll
// write_buf
// read_buf
// flock
// fallocate
// copy_file_range
// lseek

#include <fuse.h> /* fuse_main */

#include "./logger.h" /* set_log_level set_log_output logger */

static struct fuse_operations ops = {
    .getattr = o_getattr,
    .readlink = o_readlink,
    // mknod
    .mkdir = o_mkdir,
    .unlink = o_unlink,
    .rmdir = o_rmdir,
    // symlink
    .rename = o_rename,
    // link
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
    // setxattr
    // getxattr
    // listxattr
    // removexattr
    .opendir = o_opendir,
    .readdir = o_readdir,
    .releasedir = o_releasedir,
    .fsyncdir = o_fsyncdir,
    .init = o_init,
    .destroy = o_destroy,
    .access = o_access,
    // create
    .lock = o_lock,
    .utimens = o_utimens,
    // bmap
    // ioctl
    // poll
    // write_buf
    // read_buf
    // flock
    // fallocate
    // copy_file_range
    // lseek
};

int main(int argc, char **argv)
{
    set_log_level(DEBUG);
    set_log_output(stdout);

    return fuse_main(argc, argv, &ops, NULL);
}
