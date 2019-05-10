//
// Created by dragon on 10.05.19.
//

#ifndef LMON_METRICS_H
#define LMON_METRICS_H

char *getHostname();

char *kernelVersion();
double getCPULoadAvg();
int getMemAttr(char *attr);

typedef struct sDrive {

    char *blockPath;
    char *mountPoint;
    int size;
    int usage;
    int end;
} drive;

drive *getDrives();

#endif //LMON_METRICS_H
