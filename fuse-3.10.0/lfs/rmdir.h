#ifndef rmdir_h
#define rmdir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_rmdir(const char *);
#endif
