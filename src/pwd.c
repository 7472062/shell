#include <stdio.h>
#include <stdlib.h>
#include "pwd.h"

void pwd(void) {
    printf("%s\n", getenv("PWD")); // 현재 작업 디렉토리 출력
}