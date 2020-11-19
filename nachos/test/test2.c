#include "stdio.h"
#include "stdlib.h"

#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
    char prompt[] = "nachos% ";

    char buffer[BUFFERSIZE];

    int fd[20];

    memset(fd, 0, sizeof(fd));

    int amount = 1024;

    int i;
    for (i = 0; i < 100; i++)
    {
        char filename[100];
        int id = i % 20;
        printf("%d %d\n", i, id);
        memset(filename, 0, sizeof(filename));
        sprintf(filename, "%s%d", "testfile", id);
        if (fd[id] > 0)
        {
            printf("Success closed file %s\n", filename);
            close(fd[id]);
            fd[id] = 0;
        }
        else
        {
            fd[id] = open(filename);
            if (fd[id] == -1)
            {
                printf("Unable to open %s\n", filename);
                continue;
            }
            else
            {
                printf("Success opened file %s\n", filename);
                write(fd[id], filename, amount);
            }
        }
    }

    return 0;
}
