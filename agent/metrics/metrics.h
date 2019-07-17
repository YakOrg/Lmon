//
// Created by dragon on 10.05.19.
//

#ifndef LMON_METRICS_H
#define LMON_METRICS_H

#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <sys/types.h>

typedef enum types{
    IPV4,  /*0*/
    IPV6,  /*1*/
    PACKET /*2*/
}types;

typedef struct net_address {
    struct  net_address*    next;
    char*   ip_address;
    char *net_mask;
    types   type;
} net_address;

typedef struct network_interface{
    char*   interface_name;
    struct  network_interface *next;
    net_address*    addresses;
}network_interface;

typedef struct drive {
    char *blockPath;
    char *mountPoint;
    int size;
    int usage;
    int end;
} drive;

/*typedef struct sockaddr_in ip_addr;*/

typedef struct metrics {

    /* Info block */
    char *hostname;            /* Hostname */
    char *sys_name;            /* Operating system name (e.g., "Linux") */
    char *sys_release;         /* Operating system sys_release (e.g., "2.6.28") */
    char *sys_version;         /* Operating system sys_version */
    char *arch;                /* Hardware identifier */
    uint32_t uptime;           /* Seconds since boot */

    /* CPU block*/
    int processors_count;      /* Count of processors available */
    double load_average;       /* System load average */
    uint32_t processes_count;  /* Number of current processes */

    /* Memory block */
    uint64_t ram_size;         /* Total usable main memory size */
    uint64_t ram_usage;        /* Used memory size */
    uint64_t ram_shared;       /* Available shared memory  */
    uint64_t ram_buffer;       /* Memory used by buffers */
    uint64_t swap_size;        /* Total swap space size */
    uint64_t swap_usage;       /* Used swap space size */

    /* Network interfaces */
    network_interface *network_interfaces;

    /* Drives */
    drive *drives;

} metrics;

/*
 * find structure by name in structures list
 * returns structure in success
 * NULL if no match found
*/
network_interface* find_struct_by_name(network_interface* begin, char* name);

/*create and return first element of list of interfaces*/
network_interface* create_int_list(char* name);
  
network_interface* add_new_int(network_interface* in, char* name);

network_interface* get_interfaces(void);

void add_new_addr(network_interface *t, struct ifaddrs *a);

int get_base_metrics(metrics *m);

int get_advanced_metrics(metrics *m);

metrics *get_all_metrics();

void free_drives(drive *drives);

void free_metrics(metrics *m);

void free_interfaces(network_interface *in);

#endif //LMON_METRICS_H
