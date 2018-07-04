//
// Created by metablep on 7/4/18.
//

#include <zconf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char str_pid[256];


    int pid = fork();

    if (pid) {
        sprintf(str_pid, "%d", pid);

        char start[2] = { '1', '\0' };

        char* argv[]={ "Counter", str_pid, start, NULL };
        execv("Counter", argv);
    }

    if (!pid) {
        int ppid = getppid();
        int pid_ = fork();
        sprintf(str_pid, "%d", pid_? pid_ : ppid);

        char start[2] = { pid_? '2' : '3', '\0' };

        char* argv[]={ "Counter", str_pid, start, NULL };
        execv("Counter", argv);
    }

    return 0;
}