#ifndef poll_h
#define poll_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <sys/stat.h> /* mode_t */
#include <fuse.h>     /* fuse_file_info */
int o_poll(const char *, struct fuse_file_info *, struct fuse_pollhandle *, unsigned *);
#endif
