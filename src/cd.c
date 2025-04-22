#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cd.h"

void cd(const char *path) {
    const char *oldpwd = getenv("PWD"); // 현재 디렉토리를 OLDPWD로 저장

    if (path == NULL || strcmp(path, "") == 0) { // 입력된 경로가 없는 경우
        path = getenv("HOME"); // 홈 디렉토리로 이동
    } else if (strcmp(path, "-") == 0) { // cd - 처리
        path = getenv("OLDPWD"); // 이전 디렉토리로 이동
        if (path == NULL) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", path); // 이동할 디렉토리 출력
    }

    // chdir로 디렉토리 변경
    if (chdir(path) != 0) {
        perror("cd"); // 오류 메시지 출력
        return;
    }

    // PWD와 OLDPWD 환경변수 업데이트
    setenv("OLDPWD", oldpwd, 1); // 이전 디렉토리를 OLDPWD로 설정
    char *cwd = getcwd(NULL, 0); // 현재 작업 디렉토리 가져오기
    setenv("PWD", cwd, 1); // PWD 환경변수 설정
    free(cwd); // 메모리 해제
}