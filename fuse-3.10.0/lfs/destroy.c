#include "destroy.h"

void o_destroy(void *private_data)
{
    //pthread_mutex_lock(&mutex);
    logger(DEBUG, "DESTORY %p\n", private_data);
    //lfs_fflush();
    lfs_save();
    //pthread_mutex_unlock(&mutex);
}
