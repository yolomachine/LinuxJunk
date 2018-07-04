#include <stdio.h>
#include <unistd.h>
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

int main() {

    int descriptors[3][2];
    for (int i = 0; i < 3; ++i)
        pipe(descriptors[i]);
    write(descriptors[0][1], buffer, sizeof(buffer) / sizeof(*buffer));
    pid_t pid = fork();

    while (100 - sizeof(descriptors)/sizeof(*descriptors) >= strtol(buffer, NULL, 10)) {
        if (pid)
            pipeline("Parent", descriptors[0], descriptors[1]);
        else {
            pid_t c_pid = fork();
            if (c_pid)
                pipeline("Child::Parent", descriptors[1], descriptors[2]);
            else {
                pipeline("Child::Child", descriptors[2], descriptors[0]);
                raise(SIGKILL);
            };
        };
    };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 2; ++j)
            close(descriptors[i][j]);

    return 0;
}