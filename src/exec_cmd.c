#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "exec_cmd.h"

void execute_command(const char *command) {
    char *args[256]; // 명령어와 옵션을 저장할 배열
    char *cmd_copy = strdup(command); // 입력 문자열 복사
    char *token = strtok(cmd_copy, " "); // 공백 기준으로 첫 번째 토큰 추출
    int i = 0;

    while (token != NULL) {
        args[i++] = token; // 명령어와 옵션 저장
        token = strtok(NULL, " "); // 다음 토큰 추출
    }
    args[i] = NULL; // 마지막 인자는 NULL로 설정

    pid_t pid = fork(); // 자식 프로세스 생성

    if (pid == 0) { // 자식 프로세스
        execvp(args[0], args); // 명령어 실행
        perror("exec failed"); // exec 실패 시 오류 출력
        exit(1); // 실패 시 종료
    } else if (pid > 0) { // 부모 프로세스
        int status; // 자식 프로세스 종료 상태 저장
        waitpid(pid, &status, 0); // 자식 프로세스 종료 대기
        if (WIFEXITED(status)) { // 자식 프로세스가 정상 종료되었는지 확인
            int exit_code = WEXITSTATUS(status);
            if (exit_code != 0) { // 종료 코드가 0이 아닌 경우에만 출력
                printf("Command failed with exit code: %d\n", exit_code);
            }
        } 
    } else { // fork 실패
        perror("fork failed");
    }
    
    free(cmd_copy); // 복사한 문자열 메모리 해제
}