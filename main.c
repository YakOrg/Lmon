#include <stdio.h>
#include <stdlib.h>

#include "http.h"
#include "metrics.h"

#define TEMPLATE "{\"load_avg\":%.2f, \"ram_size\":%d, \"ram_usage\":%d}"

char *genPage() {
    char *template = malloc(sizeof(TEMPLATE) + 10);

    double cpuLoadAvg = getCPULoadAvg();
    int memTotal = getMemAttr("MemTotal");
    int memAvailable = getMemAttr("MemAvailable");
    int memUsed = memTotal - memAvailable;

    sprintf(template, TEMPLATE, cpuLoadAvg, memTotal, memUsed);
    return template;
}

int main() {
    startHttpServer(genPage, 8080);
}