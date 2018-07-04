#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <spandsp.h>

void handler(int signal) {
    switch (signal) {
        case SIGUSR1:
            printf("SIGUSR1 signal sent\n");
            break;
        case SIGUSR2:
            printf("SIGUSR2 signal sent\n");
            break;
        default:
            printf("External signal received: %d", signal);
            break;
    };
};

int main() {
    int pid = fork();
    signal(pid? SIGUSR1 : SIGUSR2, handler);

    int pid_ = 0;
    if (!pid) {
        pid_ = fork();
        signal(pid_ ? SIGUSR2 : SIGUSR1, handler);
    } else
        usleep(100);

    int i;
    if (pid) {
        i = 1;
        while(true) {
            printf("First: %d\n", i);
            i += 3;
            kill(pid, SIGUSR2);
            pause();
            if (i >= 100)
                break;
        }
    } else {
        pause();
        i = pid_? 2 : 3;
        while(true) {
            printf(pid_ ? "Second: %d\n" : "Third: %d\n", i);
            i += 3;
            kill((pid_ ? pid_ : getppid()), (pid_ ? SIGUSR1 : SIGUSR2));
            pause();
            if (pid_) {
                kill(getppid(), SIGUSR1);
                pause();
            }
            if (i >= 100)
                break;
        }
    }

    if (pid)
        printf("Result: %d\n", i);

    return 0;
}