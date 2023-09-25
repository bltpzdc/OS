#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void hdl1(int sig){
    int pid = getpid();
    printf("%d: got ping\n", pid);
    kill(getpid(), SIGUSR2);
}

void hdl2(int sig){
    int pid = getppid();
    printf("%d: got pong\n", pid);
}

int main(){
    struct sigaction act;
    act.sa_handler = &hdl1;
    sigfillset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);

    struct sigaction act2;
    act2.sa_handler = &hdl2;
    sigfillset(&act2.sa_mask);
    sigaction(SIGUSR2, &act2, NULL);
    

    int *a = 0;
    printf("ppip = %d\n", getpid());
    int cpid = fork();
    if (cpid != 0){
        printf("cpid = %d\n", cpid);
        kill(cpid, SIGUSR1);
    } else {
        sleep(0);
    }
    return 0;
}