#include <stdio.h>
#include <zconf.h>

int main() {
    int parent_pipe[2];
    int child_pipe[2];
    int THIRD_MEGA_PIPE[2];

    pipe(parent_pipe);
    pipe(child_pipe);
    pipe(THIRD_MEGA_PIPE);

    int pid = fork();
    int i = 0;

    if (pid) {
        close(parent_pipe[0]);
        while (i < 99) {
            printf("First: %d\n", ++i);
            write(parent_pipe[1], &i, sizeof i);
            read(THIRD_MEGA_PIPE[0], &i, sizeof i);
        }
        ++i;
        close(parent_pipe[1]);
    } else {
        int ppid = fork();
        close(ppid? THIRD_MEGA_PIPE[0] : child_pipe[0]);
        do {
            read((ppid? child_pipe[0] : parent_pipe[0]), &i, sizeof i);
            printf((ppid? "Third: %d\n" : "Second: %d\n"), ++i);
            write((ppid? THIRD_MEGA_PIPE[1] : child_pipe[1]), &i, sizeof i);
        } while (i < (ppid? 99 : 97));
        close((ppid? THIRD_MEGA_PIPE[1] : child_pipe[1]));
    }

    if (pid)
        printf("Result: %d\n", i);

    return 0;
}