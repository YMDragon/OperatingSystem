#include "syscall.h"

int main()
{
    char args[10];
    char *argv[2];
    sprintf(args, "test8_1");
    argv[0] = args;
    int process_id = exec("test8_1.coff", 1, argv);

    int status;
    int exit_status = join(process_id, &status);
    if (exit_status != 1)
    {
        printf("exit status error");
        return -1;
    }
    int status2;
    return 0;
}