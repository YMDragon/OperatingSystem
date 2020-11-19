#include "syscall.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
    const int n = 14;
    int i, fd[n], _;
    char s[n][10];
    for (i = 0; i < n; ++i)
        sprintf(s[i], "%d", i);

    for (_ = 0; _ < 2; _++)
    {
        for (i = 0; i < n; ++i)
        {
            fd[i] = creat(s[i]);
            if (fd[i] == -1)
            {
                printf("Unable to create %s\n", s[i]);
                return -1;
            }
            else
                printf("Successfully create %s\n", s[i]);
        }

        for (i = 0; i < n; ++i)
        {
            if (close(fd[i]) != 0)
            {
                printf("Unable to close %s\n", s[i]);
                return -1;
            }
            if (unlink(s[i]) != 0)
            {
                printf("Unable to remove %s\n", s[i]);
                return -1;
            }
            else
                printf("Successfully close and remove %s\n", s[i]);
        }
    }
    return 0;
}
