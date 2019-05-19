//
// Created by dragon on 10.05.19.
//

#include "metrics.h"

/*
 * find structure by name in structures list
 * returns structure in success
 * NULL if no match found
*/
network_interface* find_struct_by_name(network_interface* begin, char* name)
{
    for(;begin != NULL; begin = begin->next){
        if(strcmp(begin->interface_name, name) == 0)
            return begin;
    }

    return NULL;
}

/*create and return first element of list of interfaces*/
network_interface* create_int_list(char* name)
{
    network_interface* t = (network_interface*) malloc(sizeof(network_interface));
    t->next           = NULL;
    t->addresses      = NULL;
    t->interface_name = name;

    return t;
}

void add_new_addr(network_interface* t, struct sockaddr* a)
{
    char *ipstr;

    net_address* it          = t->addresses;
    struct sockaddr_in* addr = (struct sockaddr_in*) a;

    if(it == NULL){
        it       = (net_address*) malloc(sizeof(net_address));
        it->next = NULL;
        t->addresses = it;
        goto fill_the_gaps;
    }

    /*find last record*/
    for(;it->next != NULL; it = it->next);
    it->next = (net_address*) malloc(sizeof(net_address));
    it = it->next;
    it->next = NULL;

    fill_the_gaps:
    switch(addr->sin_family){
        case AF_INET:
            ipstr    = (char*) malloc(INET_ADDRSTRLEN);
            it->type = IPV4;
            break;
        case AF_INET6:
            ipstr = (char*) malloc(INET6_ADDRSTRLEN);
            it->type = IPV6;
            break;
        default:
            it->type = PACKET;
            it->ip_address = "0";
            return;
    }

    ipstr = (char*)inet_ntop(addr->sin_family, &(addr->sin_addr), ipstr, INET6_ADDRSTRLEN);
    if(ipstr == NULL)
        perror("INET_NTOP");

    it->ip_address = ipstr;

}

network_interface* add_new_int(network_interface* in, char* name)
{
    network_interface* t = in;

    for(;t->next != NULL; t = t->next);
    t->next = (network_interface*) malloc(sizeof(network_interface));
    t       = t->next;

    t->interface_name = name;
    t->addresses      = NULL;
    t->next           = NULL;

    return t;
}

network_interface* get_interfaces(void)
{
    struct ifaddrs *ifaddr, *ifa;
  
    /*contains pointer to first member of list*/
    network_interface* interfaces = NULL;
    /*iterator*/
    network_interface* it         = NULL;

    if(getifaddrs(&ifaddr)) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /*run through the loop of network interfaces*/
    for (ifa = ifaddr; ifa != NULL; ifa=ifa->ifa_next) {
        if(it == NULL){
            /*there are no interfaces in list*/
            /*create first element*/
            interfaces = create_int_list(ifa->ifa_name);
            /*assign him address*/
            add_new_addr(interfaces, ifa->ifa_addr);
            it = interfaces;
        }else{
            network_interface *t = find_struct_by_name(interfaces, ifa->ifa_name);
            if(t != NULL){
                add_new_addr(t, ifa->ifa_addr);
            }else{
                add_new_addr(add_new_int(interfaces, ifa->ifa_name), ifa->ifa_addr);
            }
        }

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
            memset(mountPoint, 0, sizeof(char) * lineReadSize);
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

    m->network_interfaces = get_interfaces();
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
