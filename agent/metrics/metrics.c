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
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "metrics.h"

char *getIP() {
    char *ip_address = malloc(sizeof(char) * 15);
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    memcpy(ifr.ifr_name, "enp8s0", IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    strcpy(ip_address, inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
    return ip_address;
}

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
    char base[] = ": %d kB";
    char pattern[strlen(base) + strlen(attr)];
    strcpy(pattern, attr);
    strcat(pattern, base);

    FILE *stream = fopen("/proc/meminfo", "r");
    if (stream == NULL) return -1;

    char line[512];
    while (fgets(line, 50, stream)) {
        int attrValue;
        if (sscanf(line, pattern, &attrValue)) {
            fclose(stream);
            // Convert kb to mb
            return attrValue / 1000;
        }
    }
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
    int lineReadSize = 128;
    drive *array = malloc(sizeof(drive) * 128);
    drive *iter = array;

    struct statvfs vfs;

    char line[lineReadSize],
            attrValue[lineReadSize],
            mountPoint[lineReadSize],
            blockPathTmp[lineReadSize];

    FILE *stream = fopen("/proc/mounts", "r");
    if (stream == NULL) return NULL;

    while (fgets(line, lineReadSize, stream))
        if (sscanf(line, "/dev/s%s %s", attrValue, mountPoint)) {
            sprintf(blockPathTmp, "/dev/s%s", attrValue);
            statvfs(mountPoint, &vfs);
            iter->end = 0;
            iter->mountPoint = mountPoint;
            iter->blockPath = blockPathTmp;
            iter->size = bytesToGb(vfs.f_bsize * vfs.f_blocks);
            iter->usage = iter->size - bytesToGb(vfs.f_bsize * vfs.f_bfree);
            iter++;
        }
    iter->end = 1;
    fclose(stream);
    return array;
}

int get_general_info(metrics *m) {
    struct utsname *uts = malloc(sizeof(struct utsname));
    int res = uname(uts);

    m->sysname = uts->sysname;
    m->release = uts->release;
    m->version = uts->version;
    m->machine = uts->machine;

    return res;
}

int get_advanced_info(metrics *m) {
    struct sysinfo *sys = malloc(sizeof(struct sysinfo));
    int res = sysinfo(sys);

    /*m->load = sys->loads can`t do that! Because in C cannot to assign arrays */
    m->uptime = sys->uptime;
    m->procs = sys->procs;
    m->average = sys->loads[0];
    m->totalram = sys->totalram;
    m->freeram = sys->freeram;
    m->freeswap = sys->freeswap;
    m->sharedram = sys->sharedram;
    m->bufferram = sys->bufferram;
    m->totalswap = sys->totalswap;


    return res;
}