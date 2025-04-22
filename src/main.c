#include <stdio.h>
#include <string.h>
#include "pwd.h"
#include "exec_cmd.h"
#include "print_prompt.h"

int main(void) {
    char command[256]; // 명령어를 저장할 배열

    initialize_cwd(); // 현재 디렉토리를 사용자의 홈 디렉토리로 초기화

    while (1) {
        print_prompt(); // 프롬프트 출력

        fgets(command, sizeof(command), stdin); // 명령어 입력

        command[strcspn(command, "\n")] = '\0'; // 개행 문자 제거

        if (strcmp(command, "exit") == 0) { // exit 처리
            fflush(stdout); // 출력 버퍼 비우기
            break;
        } else if (strcmp(command, "pwd") == 0) { // pwd 처리
            pwd();
        } else {
            execute_command(command); // 외부 명령어 실행
        }
    }
    return 0;
}