//
// Created by dragon on 10.05.19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "metrics.h"

double getCPULoadAvg() {
    int FileHandler;
    char FileBuffer[1024];
    float load;

    FileHandler = open("/proc/loadavg", O_RDONLY);
    if (FileHandler < 0) {
        return -1;
    }
    read(FileHandler, FileBuffer, sizeof(FileBuffer) - 1);
    load = strtof(FileBuffer, NULL);
    close(FileHandler);
    return (double) load;
}

int getMemAttr(char *attr) {
    char *base = ": %d kB";
    char *pattern = malloc(strlen(attr) + strlen(base));
    sprintf(pattern, "%s%s", attr, base);

    FILE *stream = fopen("/proc/meminfo", "r");
    if (stream == NULL) return -1;

    char *line = malloc(512);
    while (fgets(line, 50, stream)) {
        int attrValue;
        if (sscanf(line, pattern, &attrValue)) {
            fclose(stream);
            free(line);
            free(pattern);
            // Convert kb to mb
            return attrValue / 1000;
        }
    }
    free(line);
    free(pattern);
    fclose(stream);
    return -1;
}