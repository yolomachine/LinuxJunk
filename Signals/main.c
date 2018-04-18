#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

char buffer[4] = "0";

void pipeline(const char* tag, int* in_fd, int* out_fd) {
    printf("%s: ", tag);
    read(in_fd[0], buffer, sizeof(buffer) / sizeof(*buffer));
    snprintf(buffer, sizeof(buffer) / sizeof(*buffer), "%d", (int)strtol(buffer, NULL, 10) + 1);
    printf("%s\n", buffer);
    write(out_fd[1], buffer, sizeof(buffer) / sizeof(*buffer));
};

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
    int descriptors[3][2];
    for (int i = 0; i < 3; ++i)
        pipe(descriptors[i]);
    write(descriptors[0][1], buffer, sizeof(buffer) / sizeof(*buffer));
    pid_t pid = fork();

    signal(pid ? SIGUSR1 : SIGUSR2, handler);

    if (pid)
        do {
            pipeline("Parent", descriptors[0], descriptors[1]);
            if (atoi(buffer) < 100) {
                usleep(300);
                kill(pid, SIGUSR2);
                pause();
            };
        } while (atoi(buffer) < 98);
    else
        do {
            pause();
            pid_t c_pid = fork();

            if (c_pid) {
                pipeline("Child::Parent", descriptors[1], descriptors[2]);
                usleep(500);
                kill(c_pid, SIGUSR2);
                pause();
                kill(getppid(), SIGUSR1);
            }
            else {
                signal(SIGUSR2, handler);
                pause();
                pipeline("Child::Child", descriptors[2], descriptors[0]);
                usleep(500);
                kill(getppid(), SIGUSR2);
                raise(SIGKILL);
            };
        } while (atoi(buffer) < 97);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 2; ++j)
            close(descriptors[i][j]);

    return 0;
}