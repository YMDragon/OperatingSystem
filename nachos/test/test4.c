#include "stdio.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    int a[20];
    int i;
    for (i = -10; i <= 30; i++)
    {
        a[i] = i;
    }
    for (i = -10; i <= 30; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
