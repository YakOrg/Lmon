//
// Created by dragon on 10.05.19.
//

/*Please! Before makeing changes consult with Garin! (very cool guy, in my opinion)*/
/*DON`T TOUCH THIS ****************** CODE*/

#include "metrics.h"

#define SET_ZERO(a, size) memset(a, 0, size)

    typedef enum ip_type{
        IPV4,   /*0*/
        IPV6    /*1*/
    }ip_type;

    typedef struct net_address {
        struct  net_address*    next;
        char*   ip_address;
        ip_type type;
    } net_address;

    typedef struct network_interface{
        char*   interface_name;
        struct  network_interface *next;
        net_address*    addresses;
    }network_interface;

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
            else
                return NULL;
        }
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
    char* ipstr;

    net_address* it          = t->addresses;
    struct sockaddr_in* addr = (struct sockaddr_in*) a;

    /*find last record*/
    for(;it->next != NULL; it = it->next);
    it->next = (net_address*) malloc(sizeof(net_address));
    it = it->next;

    switch(addr->sin_family){
        case AF_INET:
            ipstr = (char*) calloc(INET_ADDRSTRLEN+1, 1);
            break;
        case AF_INET6:
            ipstr = (char*) calloc(INET6_ADDRSTRLEN+1, 1);
            break;
    }

    inet_ntop(addr->sin_family, &(addr->sin_addr), ipstr, strlen(ipstr));

    it->ip_address = ipstr;

}

void add_new_int(network_interface* in, char* name)
{
    network_interface* t = (network_interface*) malloc(sizeof(network_interface));

    t->interface_name = name;
    t->addresses      = NULL;
    t->next           = NULL;

    in->next          = t;
}

network_interface* get_interfaces(void)
{
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in* addr;
    char ipstr[INET6_ADDRSTRLEN];
    /*contains pointer to first member of list*/
    network_interface* interfaces = NULL;
    /*iterator*/
    network_interface* it         = NULL;

    /*memset(ipstr, 0, sizeof(ipstr));*/
    SET_ZERO(ipstr, sizeof(ipstr));

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
            /*add_new_addr();*/
            /*TODO*/
        }else{
            /*add_new_int();
            add_new_addr();*/
            /*TODO*/
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