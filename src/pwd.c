#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pwd.h"

void initialize_cwd(void) { // 현재 디렉토리를 사용자의 홈 디렉토리로 초기화
    chdir(getenv("HOME")); // 현재 디렉토리 변경
    setenv("PWD", getenv("HOME"), 1); // PWD 환경변수 설정
}

int pwd(void) { // 현재 작업 디렉토리 출력
    printf("%s\n", getenv("PWD"));
    return 0; 
}