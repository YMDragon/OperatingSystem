#ifndef ioctl_h
#define ioctl_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <fuse.h>     /* fuse_file_info */
int o_ioctl(const char *, int, void *, struct fuse_file_info *, unsigned int, void *);
#endif
