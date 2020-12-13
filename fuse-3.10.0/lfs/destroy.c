#include "destroy.h"

void o_destroy(void *private_data)
{
    logger(DEBUG, "DESTORY %p\n", private_data);
    //lfs_fflush();
    lfs_save();
}
