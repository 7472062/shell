#include <stdio.h>
#include <stdlib.h>
#include "pwd.h"

void pwd(void) {
    printf("%s\n", getenv("PWD"));
}