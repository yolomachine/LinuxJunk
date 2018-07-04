//
// Created by metablep on 7/4/18.
//

#ifndef INOTIFY_FOR_MV_UTILS_H
#define INOTIFY_FOR_MV_UTILS_H

#include "event_queue.h"

void handle_event(queue_entry_t event);
int event_check(int fd);
int process_inotify_events(queue_t q, int fd, int pid);
int watch(int fd, const char* dirname, uint32_t mask);
int close_inotify_fd(int fd);
int open_inotify_fd();

#endif //INOTIFY_FOR_MV_UTILS_H
