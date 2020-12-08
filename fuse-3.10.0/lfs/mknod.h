#ifndef mknod_h
#define mknod_h
#include "./logger.h"  /* logger */
#include "./prefix.h"  /* prefix */
#include <sys/stat.h>  /* mode_t */
#include <sys/types.h> /* dev_t */
int o_mknod(const char *, mode_t, dev_t);
#endif
