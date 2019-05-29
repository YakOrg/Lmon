//
// Created by dragon on 26.05.19.
//

#include "utils.h"

int int_len(int i) {
    return i > 0 ? 1 + int_len(i / 10) : 0;
}

char *itoa(int i) {
    char *str = malloc((int_len(i) + 1) * sizeof(char));
    sprintf(str, "%d", i);
    return str;
}