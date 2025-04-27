#ifndef HANDLE_SIG_H
#define HANDLE_SIG_H

#include <sys/types.h>

#define MAX_BACKGROUND_PROCESSES 256

extern pid_t background_pids[MAX_BACKGROUND_PROCESSES];
extern int background_count;

void add_background_pid(pid_t pid);
void remove_background_pid(pid_t pid);

void setup_signal_handler(void);

#endif