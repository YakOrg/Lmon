// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

//
// Created by dragon on 26.05.19.
//

#include "utils.h"

void* malloc_wr(size_t len) {
    void* result = malloc(len);
    if (result == 0) {
        printf("memory allocation failed\n");
        exit(1);
    }
    return result;
}

int int_len(int i) {
    return i > 0 ? 1 + int_len(i / 10) : 0;
}

char *itoa(int i) {
    char *str = malloc_wr((int_len(i) + 1) * sizeof(char));
    sprintf(str, "%d", i);
    return str;
}
