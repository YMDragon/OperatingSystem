#include "syscall.h"

int main()
{
    char args[10];
    char *argv[2];
    sprintf(args, "test9_2");
    argv[0] = args;
    int process_id = exec("test9_2.coff", 1, argv);

    printf("test9_1 fin\n");
    return process_id;
}