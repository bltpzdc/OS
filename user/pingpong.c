#include "kernel/types.h"
#include "user/user.h"

#define PING "ping"
#define PONG "pong"

void 
main(){
    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);

    int pr = fork();
    int pid = getpid();

    if (pr > 0){
        close(p1[0]);
        write(p1[1], PING, sizeof(PING));
        close(p1[1]);
        close(p2[1]);
        char *buf[sizeof(PONG)];
        read(p2[0], buf, sizeof(buf));
        close(p2[0]);
        fprintf(1, "%d: got %s\n", pid, buf);
    } else if (pr == 0){
        close(p1[1]);
        char *buf[sizeof(PING)];
        read(p1[0], buf, sizeof(buf));
        close(p1[0]);
        fprintf(1, "%d: got %s\n", pid, buf);
        close(p2[0]);
        write(p2[1], PONG, sizeof(PONG));
        close(p2[1]);
    } else {
        fprintf(2, "Error during fork.");
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
    }
    exit(0);
}