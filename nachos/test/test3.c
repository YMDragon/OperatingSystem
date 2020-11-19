#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    int i;

    printf("%d arguments\n", argc);

    for (i = 0; i < argc; i++)
        printf("arg %d: %s\n", i, argv[i]);

    int n = atoi(argv[1]);
    int pid, status;

    for (; n > 0; n--)
    {
        printf("%d\n", n);
        argv[1][0]--;
        pid = exec("test3.coff", argc, argv);
        join(pid, &status);
    }
    return 0;
}
