/*
* Created by slagger on 5/11/19.
*/

#include "metrics.h"

int get_general_info(metrics* m)
{
    struct utsname* uts = malloc(sizeof(struct utsname));
    int res = uname(uts);

    m->sysname = uts->sysname;
    m->release = uts->release;
    m->version = uts->version;
    m->machine = uts->machine;

    return res;
};

int get_advanced_info(metrics* m)
{
    struct sysinfo* sys = malloc(sizeof(struct sysinfo));
    int res = sysinfo(sys);

    /*m->load = sys->loads can`t do that! Because in C cannot to assign arrays */
    m->uptime   = sys->uptime;
    m->procs    = sys->procs;
    m->average  = sys->loads[0];
    m->totalram = sys->totalram;
    m->freeram  = sys->freeram;
    m->freeswap = sys->freeswap;
    m->sharedram = sys->sharedram;
    m->bufferram = sys->bufferram;
    m->totalswap = sys->totalswap;


    return res;
};