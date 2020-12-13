#include "../lfs.h"
#include <stdlib.h>

int main()
{
    int i;
    char buf[100];
    for (i = 0; i < 1000; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "mkdir test%d", i);
        system(buf);
    }
    return 0;
}