#ifndef rename_h
#define rename_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_rename(const char *, const char *, unsigned int);
#endif
