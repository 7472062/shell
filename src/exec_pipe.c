#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "exec_cmd.h"
#include "exec_pipe.h"

int execute_pipeline(char *commands[], int index, int input_fd) {
    int pipefd[2];
    if (commands[index + 1] != NULL) { // 다음 명령어가 있다면 파이프 생성
        if (pipe(pipefd) == -1) {
            perror("pipe failed");
            exit(1);
        }
    }

    pid_t pid = fork();
    if (pid == 0) { // 자식 프로세스
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO); // 이전 명령어의 출력 연결
            close(input_fd); // 이전 파이프의 읽기 끝 닫기
        }
        if (commands[index + 1] != NULL) {
            dup2(pipefd[1], STDOUT_FILENO); // 현재 명령어의 출력 연결
            close(pipefd[0]);
            close(pipefd[1]);
        }
        execute_command(commands[index], 1); // 명령어 실행
    } else if (pid > 0) { // 부모 프로세스
        wait(NULL); // 자식 프로세스 종료 대기
        if (commands[index + 1] != NULL) {
            close(pipefd[1]); // 현재 파이프의 쓰기 끝 닫기
            execute_pipeline(commands, index + 1, pipefd[0]); // 다음 명령어 처리
        }
    } else {
        perror("fork failed");
    }
    return 0;
}