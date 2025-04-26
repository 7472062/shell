#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "exec_cmd.h"
#include "pwd.h"
#include "cd.h"

int execute_command(const char *command) { // 명령어 실행 함수
    char *args[256]; // 명령어와 옵션을 저장할 배열
    char *cmd_copy = strdup(command); // 입력 문자열 복사
    cmd_copy[strcspn(cmd_copy, "\n")] = '\0'; // 개행 문자 제거
    int i = 0, exit_code = -1;

    // 백그라운드 실행 여부 확인
    int is_background = 0;
    if (cmd_copy[strlen(cmd_copy) - 1] == '&') { // 명령어가 &로 끝나는 경우
        is_background = 1;
        cmd_copy[strlen(cmd_copy) - 1] = '\0'; // & 제거
    }

    char *token = strtok(cmd_copy, " "); // 공백 기준으로 첫 번째 토큰 추출

    // 명령어와 옵션 분리 저장
    while (token != NULL) {
        args[i++] = token; 
        token = strtok(NULL, " "); // 다음 토큰 추출
    }
    args[i] = NULL; // 마지막 인자는 NULL로 설정

    pid_t pid = 0;

    if (is_background) {
        pid = fork(); // 자식 프로세스 생성
        if (pid > 0) { // 부모 프로세스인 경우
            fflush(stdout);
            printf("Background process PID: %d\n", pid); // 백그라운드 프로세스 PID 출력
            goto SKIP_CHILD;
        } else if (pid < 0) { // fork 실패
            perror("fork failed");
            free(cmd_copy);
            return exit_code;
        }
    }

    // 쉘 내부 명령어 처리
    if (strcmp(args[0], "pwd") == 0) { // pwd 처리
        exit_code = pwd(); // pwd 명령어 실행
        free(cmd_copy); 
        if (is_background) {
            exit(exit_code);
        } else {
            return exit_code;
        }
    } else if (strcmp(args[0], "cd") == 0) { // cd 처리
        exit_code = cd(args[1]);
        free(cmd_copy); 
        if (is_background) {
            exit(exit_code);
        } else {
            return exit_code;
        }
    }

    if (!is_background) {
        pid = fork(); // 자식 프로세스 생성
    }

    if (pid == 0) { // 자식 프로세스
        execvp(args[0], args);
        perror("exec failed");
        free(cmd_copy); 
        exit(1);
    } else if (pid > 0) { // 부모 프로세스
        int status; // 자식 프로세스 종료 상태를 저장할 변수
        waitpid(pid, &status, 0); // 자식 프로세스 종료 대기
        if (WIFEXITED(status)) { // 자식 프로세스가 정상 종료되었는지 확인
            exit_code = WEXITSTATUS(status); // 종료 코드 저장
            if (exit_code != 0) { // 종료 코드가 0이 아닌 경우
                printf("Command failed with exit code: %d\n", exit_code);
            }
        } 
    } else { // fork 실패
        perror("fork failed");
    }
    
    SKIP_CHILD:
    free(cmd_copy);
    return exit_code; // 종료 코드 반환
}

void process_command(const char *input) { // 입력문 처리 함수
    char *cmd_copy = strdup(input); // 입력 문자열 복사
    char *commands[256]; // 명령어를 저장할 배열
    char *operators[256]; // 연산자를 저장할 배열
    int cmd_index = 0, op_index = 0;

    for (int i = 0; cmd_copy[i] != '\0';) {

        // 명령어 추출
        char *start = &cmd_copy[i];
        while (cmd_copy[i] != '\0' && cmd_copy[i] != ';' && cmd_copy[i] != '|' && cmd_copy[i] != '&') {
            i++;
        }

        // 연산자 추출 및 저장
        if ((cmd_copy[i] == ';' || cmd_copy[i] == '|' || cmd_copy[i] == '&') && start != &cmd_copy[i]) {
            if (cmd_copy[i] == '|' && cmd_copy[i + 1] == '|') { // || 처리
                operators[op_index++] = "||"; // 연산자 저장
                cmd_copy[i] = '\0'; // 명령어 끝에 NULL 추가
                commands[cmd_index++] = start; // 명령어 저장
                i += 2;
            } else if (cmd_copy[i] == '&' && cmd_copy[i + 1] == '&') { // && 처리
                operators[op_index++] = "&&";
                cmd_copy[i] = '\0';
                commands[cmd_index++] = start;
                i += 2;
            } else if (cmd_copy[i] == '&' && cmd_copy[i + 1] == ';') {
                operators[op_index++] = ";";
                cmd_copy[i + 1] = '\0';
                commands[cmd_index++] = start;
                i += 2;
            } else if (cmd_copy[i] == '&') {
                commands[cmd_index++] = start;
                i++;
            } else { // ; 또는 |
                operators[op_index++] = (cmd_copy[i] == ';') ? ";" : "|";
                cmd_copy[i] = '\0'; 
                commands[cmd_index++] = start;
                i++;
            }
        } else if (cmd_copy[i] == '\0') { // cmd_copy[i] == '\0'인 경우
            commands[cmd_index++] = start; 
        } else {
            fprintf(stderr, "Error: Invalid command\n");
            free(cmd_copy); // 복사한 문자열 메모리 해제
            return; // 오류 발생 시 종료
        }
    }
    commands[cmd_index] = NULL; // 명령어 배열 종료
    operators[op_index] = NULL; // 연산자 배열 종료

    // 명령어 실행
    int exit_code[cmd_index]; // 각 명령어의 종료 코드를 저장할 배열
    memset(exit_code, -1, sizeof(exit_code)); // 배열 초기화

    exit_code[0] = execute_command(commands[0]); // 첫 번째 명령어 실행
    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(operators[i], ";") == 0) {
            exit_code[i + 1] = execute_command(commands[i + 1]); // 다음 명령어 실행
        } else if (strcmp(operators[i], "||") == 0 && exit_code[i] != 0) { // 이전 명령어 실패시
            exit_code[i + 1] = execute_command(commands[i + 1]);
        } else if (strcmp(operators[i], "&&") == 0 && exit_code[i] == 0) { // 이전 명령어 성공시
            exit_code[i + 1] = execute_command(commands[i + 1]);  
        } else if (strcmp(operators[i], "|") == 0) {
            // 파이프 처리 (추가 구현 필요)
        }
    }
    free(cmd_copy); // 복사한 문자열 메모리 해제
}
