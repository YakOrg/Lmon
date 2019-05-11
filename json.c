/*
* Created by slagger on 5/11/19.
*/

#include "json.h"

json_t* make_json(metrics* m)
{
    json_t* json = json_object();
    json_t* sysname = json_string(m->sysname);
    json_t* release = json_string(m->release);
    json_t* version = json_string(m->version);
    json_t* machine = json_string(m->machine);

    json_t* uptime = json_integer(m->uptime);
    json_t* totalram = json_integer(m->totalram);
    json_t* freeram = json_integer(m->freeram);
    json_t* sharedram = json_integer(m->sharedram);
    json_t* bufferram = json_integer(m->bufferram);
    json_t* totalswap = json_integer(m->totalswap);
    json_t* freeswap = json_integer(m->freeswap);
    json_t* procs = json_integer(m->procs);
    json_t* average = json_integer(m->average);

    json_object_set(json, "sysname", sysname);
    json_object_set(json, "release", release);
    json_object_set(json, "version", version);
    json_object_set(json, "machine id", machine);

    json_object_set(json, "uptime", uptime);
    json_object_set(json, "average", average);
    json_object_set(json, "totalram", totalram);
    json_object_set(json, "freeram", freeram);
    json_object_set(json, "sharedram", sharedram);
    json_object_set(json, "bufferram", bufferram);
    json_object_set(json, "totalswap", totalswap);
    json_object_set(json, "freeswap", freeswap);
    json_object_set(json, "procs", procs);

    return json;
}



