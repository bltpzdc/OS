#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int pid = fork();
    if (pid == 0) {
        exit(10);
    }
    wait(0);
    exit(0);
}