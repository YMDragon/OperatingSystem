#include "stdio.h"
#include "stdlib.h"

#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
    char prompt[] = "nachos% ";

    char buffer[BUFFERSIZE];

    int fd[20];

    int amount = 1024;

    int i;

    for (i = 0; i < 20; i++)
    {
        char filename[100];
        memset(filename, 0, sizeof(filename));
        sprintf(filename, "%s%d", "testfile", i);
        fd[i] = open(filename);
        if (fd[i] == -1)
        {
            printf("Unable to open %s\n", filename);
            continue;
        }
        else
        {
            write(fd[i], filename, amount);
        }
    }

    return 0;
}
