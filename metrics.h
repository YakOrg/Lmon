/*
* Created by slagger on 5/11/19.
*/

#ifndef UNTITLED2_METRICS_H
#define UNTITLED2_METRICS_H

#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

typedef struct metrics {
    /*general information*/
    char*    sysname;   /* Operating system name (e.g., "Linux") */
    char*    release;   /* Operating system release (e.g., "2.6.28")*/
    char*    version;   /* Operating system version */
    char*    machine;   /* Hardware identifier */

    /*advanced information*/
    uint32_t uptime;    /* Seconds since boot */
    uint32_t average;   /* load averages */
    uint64_t totalram;  /* Total usable main memory size */
    uint64_t freeram;   /* Available memory size */
    uint64_t sharedram; /* Available shared memory  */
    uint64_t bufferram; /* Memory used by buffers */
    uint64_t totalswap; /* Total swap space size */
    uint64_t freeswap;  /* Swap space still available */
    uint32_t procs;     /* Number of current processes */
}metrics;

int get_general_info(metrics* m);
int get_advanced_info(metrics* m);

#endif
