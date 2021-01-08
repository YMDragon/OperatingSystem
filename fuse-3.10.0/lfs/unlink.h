#ifndef unlink_h
#define unlink_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_unlink(const char *path);
#endif
