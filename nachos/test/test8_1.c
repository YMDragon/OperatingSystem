#include "syscall.h"

int main()
{
    char args[10];
    char *argv[2];
    sprintf(args, "test8_2");
    argv[0] = args;
    int process_id = exec("test8_2.coff", 1, argv);

    printf("test8_1 fin\n");
    return process_id;
}