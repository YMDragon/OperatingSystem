#ifndef mkdir_h
#define mkdir_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
#include <sys/stat.h> /* mode_t */
#include "directory.h"
#include "lfs.h"
#include "mutex.h"
int o_mkdir(const char *, mode_t);
#endif
