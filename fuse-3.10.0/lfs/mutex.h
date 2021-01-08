#ifndef mutex_h
#define mutex_h
#include <pthread.h>
#define MAX_FILES 100 * 1024
pthread_mutex_t mutex[MAX_FILES];
#endif