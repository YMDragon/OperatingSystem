#ifndef access_h
#define access_h
#include "./logger.h" /* logger */
#include "prefix.h"   /* prefix */
#include <sys/stat.h>
#include "lfs.h"
#include "directory.h"
#include "permission.h"
#include "mutex.h"
int o_access(const char *, int);
#endif
