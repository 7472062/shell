#include <stdio.h>
#include <string.h>
#include "init_pwd.h"
#include "pwd.h"
#include "exec_cmd.h"

int main(void) {
    char command[256]; // 명령어를 저장할 배열

    initialize_pwd(); // PWD 초기화

    while (1) {
        printf("shell> "); // 프롬프트 출력
        
        if (fgets(command, sizeof(command), stdin) == NULL) { // 명령어 입력
            break; // 입력 오류 시 종료
        }

        command[strcspn(command, "\n")] = '\0'; // 개행 문자 제거

        if (strcmp(command, "exit") == 0) { // exit 처리
            break;
        } else if (strcmp(command, "pwd") == 0) { // pwd 처리
            pwd();
        } else {
            execute_command(command); // 외부 명령어 실행
        }
    }
    return 0;
}