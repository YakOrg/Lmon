//
// Created by dragon on 10.05.19.
//

#include "metrics.h"

network_interface* get_interfaces(void){
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in* addr;
    char ipstr[INET6_ADDRSTRLEN];
    
    network_interface* interfaces = NULL;
    network_interface* it         = NULL;

    memset(ipstr, 0, sizeof(ipstr));

    if(getifaddrs(&ifaddr)) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa=ifa->ifa_next) {
        if(it == NULL){
            it = (network_interface*) malloc(sizeof(network_interface));
            it->next = NULL;
            interfaces = it;
        }else {
            it->next = (network_interface *) malloc(sizeof(network_interface));
            it = it->next;
            it->next = NULL;
        }

        addr = (struct sockaddr_in *) ifa->ifa_addr;
        inet_ntop(addr->sin_family, &(addr->sin_addr), ipstr, INET6_ADDRSTRLEN);

        if(ifa->ifa_addr->sa_family == AF_INET){
            memcpy(it->ipv4_address, ipstr, INET_ADDRSTRLEN);
            it->ipv6_address[0] = -1;
        }else{
            memcpy(it->ipv6_address, ipstr, INET6_ADDRSTRLEN);
            it->ipv4_address[0] = -1;
        }

        it->interface_name = ifa->ifa_name;
    }
    
    return interfaces;
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

int bytesToMb(unsigned long bytes) {
    return bytes / 1024 / 1024;
}

int bytesToGb(unsigned long bytes) {
    return bytesToMb(bytes) / 1024;
}

drive *getDrives() {
    int lineReadSize = 128;
    drive *array = malloc(sizeof(drive) * 128);
    drive *iter = array;

    struct statvfs vfs;

    char line[lineReadSize],
            attrValue[lineReadSize],
            mountPointTmp[lineReadSize];

    FILE *stream = fopen("/proc/mounts", "r");
    if (stream == NULL) return NULL;

    while (fgets(line, lineReadSize, stream))
        if (sscanf(line, "/dev/s%s %s", attrValue, mountPointTmp)) {
            char *blockPathTmp = malloc(sizeof(char) * lineReadSize);
            char *mountPoint = malloc(sizeof(char) * lineReadSize);
            memcpy(mountPoint, mountPointTmp, strlen(mountPointTmp) * sizeof(char));
            sprintf(blockPathTmp, "/dev/s%s", attrValue);
            statvfs(mountPointTmp, &vfs);
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

int get_base_metrics(metrics *m) {
    struct utsname *uts = malloc(sizeof(struct utsname));
    int res = uname(uts);

    m->hostname = uts->nodename;
    m->sys_name = uts->sysname;
    m->sys_release = uts->release;
    m->sys_version = uts->version;
    m->arch = uts->machine;

    return res;
}

int get_advanced_metrics(metrics *m) {
    struct sysinfo *sys = malloc(sizeof(struct sysinfo));
    int res = sysinfo(sys);

    m->local_ip = getIP();
    m->uptime = sys->uptime;

    m->processes_count = sys->procs;
    m->load_average = getCPULoadAvg();
    m->processors_count = get_nprocs();

    m->ram_size = bytesToMb(sys->totalram);
    m->ram_usage = bytesToMb(sys->totalram - sys->freeram);
    m->ram_shared = bytesToMb(sys->sharedram);
    m->ram_buffer = bytesToMb(sys->bufferram);

    m->swap_size = bytesToMb(sys->totalswap);
    m->swap_usage = bytesToMb(sys->totalswap - sys->freeswap);

    m->drives = getDrives();

    return res;
}