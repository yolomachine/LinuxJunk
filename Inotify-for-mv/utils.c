#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

#include <sys/inotify.h>
#include <libnet.h>

#include "event_queue.h"
#include "utils.h"

extern int keep_running;

int open_inotify_fd() {
    return inotify_init();
}

int close_inotify_fd(int fd) {
    return close(fd);
}

void handle_event(queue_entry_t event) {
    char* cur_event_file_or_dir = (event->inot_ev.mask & IN_ISDIR) ? "dir" : "file";
    switch (event->inot_ev.mask & (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED)) {
        case IN_MOVED_FROM:
        case IN_MOVED_TO:
        case IN_MOVE_SELF:
            printf("Catched 'mv' call for %s\n", cur_event_file_or_dir);
            keep_running = 0;
            break;

        default:
            break;
    }
}

void handle_events(queue_t q) {
    queue_entry_t event;
    while (!queue_empty(q)) {
        event = queue_pop(q);
        handle_event(event);
        free(event);
    }
}

int read_events(queue_t q, int fd) {
    char buffer[16384];
    size_t buffer_i = 0;
    struct inotify_event* pevent;
    queue_entry_t event;
    ssize_t r;
    size_t event_size, q_event_size;
    int count = 0;

    if ((r = read(fd, buffer, 16384)) > 0)
        while(buffer_i < r) {
            pevent = (struct inotify_event*) &buffer[buffer_i];
            event_size = offsetof(struct inotify_event, name) + pevent->len;
            q_event_size = offsetof(struct queue_entry, inot_ev.name) + pevent->len;
            event = malloc(q_event_size);
            memmove(&(event->inot_ev), pevent, event_size);
            queue_push(event, q);
            buffer_i += event_size;
            ++count;
        }
    else
        return r;

    return count;
}

int event_check(int fd) {
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    return select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
}

int process_inotify_events(queue_t q, int fd, int pid)  {
    while (keep_running) {
        kill(pid, SIGUSR1);
        if (event_check(fd) > 0)
            if (read_events(q, fd) >= 0) handle_events(q);
    }

    kill(pid, SIGUSR2);
    return 0;
}

int watch(int fd, const char* path, uint32_t mask) {
    return inotify_add_watch(fd, path, mask);
}