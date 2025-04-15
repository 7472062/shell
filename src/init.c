#include <stdio.h>
#include <stdlib.h>
#include "init.h"

void initialize_pwd(void) {
    setenv("PWD", getenv("HOME"), 1); // 현재 디렉토리를 사용자의 홈 디렉토리로 초기화
}