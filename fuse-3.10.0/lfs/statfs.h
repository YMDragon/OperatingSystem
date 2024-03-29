#ifndef statfs_h
#define statfs_h
#include "./logger.h"    /* logger */
#include "./prefix.h"    /* prefix */
#include <sys/statvfs.h> /* statvfs */
int o_statfs(const char *, struct statvfs *);
#endif
