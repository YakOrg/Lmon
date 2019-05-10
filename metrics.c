//
// Created by dragon on 10.05.19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>

#include "metrics.h"

char *getHostname() {
    char *hostname = malloc(1024);
    gethostname(hostname, 1024);
    return hostname;
}

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

char *kernelVersion() {
    struct utsname buf;
    int ret = uname(&buf);
    char *version = malloc(strlen(buf.release));
    strcat(version, buf.release);
    return version;
}

int bytesToGb(unsigned long bytes) {
    return bytes / 1024 / 1024 / 1024;
}

drive *getDrives() {
    FILE *stream = fopen("/proc/mounts", "r");
    if (stream == NULL) return NULL;
    int lineReadSize = 50;
    drive *array = malloc(lineReadSize * 128);
    drive *iter = array;

    char *line = malloc(lineReadSize);
    while (fgets(line, lineReadSize, stream)) {
        char *attrValue = malloc(lineReadSize);
        char *mountPoint = malloc(128);
        if (sscanf(line, "/dev/s%s %s", attrValue, mountPoint)) {
            char *blockPath = malloc(10);
            sprintf(blockPath, "/dev/s%s", attrValue);
            iter->mountPoint = mountPoint;
            iter->blockPath = blockPath;
            struct statvfs vfs;
            statvfs(mountPoint, &vfs);
            iter->size = bytesToGb(vfs.f_bsize * vfs.f_blocks);
            iter->usage = iter->size - bytesToGb(vfs.f_bsize * vfs.f_bfree);
            iter++;
        }
    }
    free(line);
    fclose(stream);
    return array;
}

