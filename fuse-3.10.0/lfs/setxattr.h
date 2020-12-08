#ifndef setxattr_h
#define setxattr_h
#include "./logger.h" /* logger */
#include "./prefix.h" /* prefix */
int o_setxattr(const char *, const char *, const char *, size_t, int);
#endif
