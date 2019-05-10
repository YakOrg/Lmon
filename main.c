#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <sys/sysinfo.h>

#include "http.h"

#define TEMPLATE "{\"load_avg\":%f, \"ram_size\":%d, \"ram_usage\":%d}"

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
            return attrValue;
        }
    }
    free(line);
    free(pattern);
    fclose(stream);
    return -1;
}

char *genPage() {
    char *template = malloc(sizeof(TEMPLATE) + 10);
    int memSize = getMemAttr("MemTotal") / 1000;
    sprintf(template, TEMPLATE,
            getCPULoadAvg(),
            memSize,
            memSize - getMemAttr("MemAvailable") / 1000);
    return template;
}

int main() {
    startServer(genPage, 8080);
}