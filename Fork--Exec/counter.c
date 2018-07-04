#include <stdio.h>
#include <libnet.h>
#include <spandsp.h>
#include <signal.h>

void handler(int signal) {
    switch (signal) {
        case SIGUSR1:
            printf("SIGUSR1 signal sent\n");
            break;
        default:
            printf("External signal received: %d", signal);
            break;
    };
};


int main(int argc, char* argv[]) {
    int pid = (int) strtol(argv[1], NULL, 10);
    int i = (int) strtol(argv[2], NULL, 10);

    signal(SIGUSR1, handler);

    if (i > 1)
        pause();
    else
        usleep(100);

    while(true) {
        printf("PID %d: %d\n", getpid(), i);
        i += 3;
        kill(pid, SIGUSR1);
        pause();
        if (i >= 100)
            break;
    }
        printf("Result: %d\n", i);

    return 0;
}