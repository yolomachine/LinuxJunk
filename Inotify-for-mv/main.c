//
// Created by metablep on 7/4/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <signal.h>

#include "event_queue.h"
#include "utils.h"

int keep_running;
int fd;

void handler(int sig) {
    if (sig == SIGUSR2)
        keep_running = 0;
}

int main(int argc, char* argv[]) {
    fd = open("test", O_CREAT | O_RDWR);
    keep_running = 1;

    int pid = fork();

    if (pid) {
        usleep(1000);
        int inotify_fd;

        if (signal(SIGINT, handler) == SIG_IGN)
            signal(SIGINT, SIG_IGN);

        inotify_fd = open_inotify_fd();
        if (inotify_fd > 0) {
            queue_t q = queue_create();
            int wd = watch(inotify_fd, "test", IN_ALL_EVENTS);
            if (wd > 0)
                process_inotify_events(q, inotify_fd, pid);
            close(fd);
            printf("\nTerminating\n");

            close_inotify_fd(inotify_fd);
            queue_destroy(q);
        }
    }
    else {
        struct sigaction act;

        memset(&act, 0, sizeof(act));
        act.sa_handler = handler;

        sigaction(SIGUSR1,  &act, 0);
        sigaction(SIGUSR2, &act, 0);

        char buffer[3] = { '1', '\n', '\0' };
        printf("Write initiated\n");
        while (keep_running) {
            pause();
            write(fd, buffer, strlen(buffer));
            usleep(100);
        }
        printf("Write interrupted\n");
    }

    return 0;
}

