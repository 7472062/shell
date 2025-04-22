#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "print_prompt.h"

void print_prompt(void) {

    // 현재 디렉토리명 추출
    char *directory_name;
    int slashCount = 0;
    const char *p = getenv("PWD");

    while ((p = strchr(p, '/')) != NULL) {
        slashCount++; // '/'를 찾을 때마다 카운트 증가
        p++;     // 포인터를 다음 문자로 이동
    }

    if (slashCount == 1) { // 현재 디렉토리가 루트 혹은 루트 하위 디렉토리인 경우
        directory_name = getenv("PWD"); 
    } else if (strcmp(getenv("PWD"), getenv("HOME")) == 0) { // 현재 디렉토리가 홈 디렉토리인 경우
        directory_name = "~"; 
    } else {
        directory_name = strrchr(getenv("PWD"), '/') + 1; // 마지막 '/' 이후의 문자열
    }

    // 유저 이름 추출
    char *user = getenv("USER");

    // 호스트 이름 추출
    char *hostname = getenv("HOSTNAME");

    // 프롬프트 출력
    printf("[%s@%s:%s]%c ", user, hostname, directory_name, strcmp(user, "root") ? '$' : '#');
    fflush(stdout); // 출력 버퍼 비우기
}