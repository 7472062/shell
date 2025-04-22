#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pwd.h"

void initialize_cwd(void) { // 현재 디렉토리를 사용자의 홈 디렉토리로 초기화
    chdir(getenv("HOME"));
}
void pwd(void) { // 현재 작업 디렉토리 출력
    char *cwd = getcwd(NULL, 0); // 현재 작업 디렉토리 가져오기
    printf("%s\n", cwd); 
    free(cwd); // 메모리 해제
}