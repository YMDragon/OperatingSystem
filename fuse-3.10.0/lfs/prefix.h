#ifndef prefix_h
#define prefix_h
#include "./logger.h" /* logger */
#include <string.h>   /* strlen strcat strcpy */
#include <unistd.h>   /* getcwd */
#include <stdlib.h>   /* malloc free */
#include <string.h>   /* strcat strcpy */
#include <errno.h>    /* errno */
// The current working directory for the process
char *prefix;
void generate_prefix();
#endif
