#ifndef HANDLE_SIG_H
#define HANDLE_SIG_H

#include <sys/types.h>

#define MAX_BACKGROUND_PROCESSES 256

extern pid_t BACKGROUND_PIDS[MAX_BACKGROUND_PROCESSES];
extern int BACKGROUND_COUNT;

void add_background_pid(pid_t pid);
void remove_background_pid(pid_t pid);

void setup_signal_handler(void);

#endif