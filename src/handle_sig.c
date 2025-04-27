#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "handle_sig.h"

#define MAX_BACKGROUND_PROCESSES 256

// 백그라운드 프로세스 리스트
pid_t background_pids[MAX_BACKGROUND_PROCESSES];
int background_count = 0;

// 백그라운드 PID 추가 함수
void add_background_pid(pid_t pid) {
    if (background_count < MAX_BACKGROUND_PROCESSES) {
        background_pids[background_count++] = pid;
    }
}

// 백그라운드 PID 제거 함수
void remove_background_pid(pid_t pid) {
    for (int i = 0; i < background_count; i++) {
        if (background_pids[i] == pid) {
            // PID를 리스트에서 제거
            for (int j = i; j < background_count - 1; j++) {
                background_pids[j] = background_pids[j + 1];
            }
            background_count--;
            break;
        }
    }
}

// SIGCHLD 시그널 핸들러
void sigchld_handler(int signo) {
    (void)signo; // 시그널 번호 사용하지 않음
    pid_t pid;
    int status;

    //종료된 자식 프로세스 처리
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < background_count; i++) {
            if (background_pids[i] == pid) {
                printf("done %d\n", pid);
                remove_background_pid(pid); // 리스트에서 제거
                break;
            }
        }
    }
}

// SIGCHLD 핸들러 설정 함수
void setup_signal_handler(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // 시그널 처리 중 중단된 시스템 호출 재시작
    sigaction(SIGCHLD, &sa, NULL);
}