/*
* Created by slagger on 5/11/19.
*/

#include "json.h"

json_t *make_json(metrics *m) {
    json_t* json = json_object();

    // Info block
    json_t *info_block = json_object();

    json_object_set(info_block, "hostname", json_string(m->hostname));
    json_object_set(info_block, "uptime", json_integer(m->uptime));

    json_t *info_sys_block = json_object();
    json_object_set(info_sys_block, "name", json_string(m->sys_name));
    json_object_set(info_sys_block, "release", json_string(m->sys_release));
    json_object_set(info_sys_block, "version", json_string(m->sys_version));
    json_object_set(info_sys_block, "arch", json_string(m->arch));
    json_object_set(info_block, "system", info_sys_block);

    json_object_set(json, "info", info_block);


    // CPU block
    json_t *cpu_block = json_object();
    json_object_set(cpu_block, "load_avg", json_real(m->load_average));
    json_object_set(cpu_block, "processes_count", json_integer(m->processes_count));
    json_object_set(json, "cpu", cpu_block);


    // Memory block
    json_t *memory_block = json_object();
    json_object_set(memory_block, "size", json_integer(m->ram_size));
    json_object_set(memory_block, "usage", json_integer(m->ram_usage));
    json_object_set(memory_block, "shared", json_integer(m->ram_shared));
    json_object_set(memory_block, "buffer", json_integer(m->ram_buffer));

    json_t *memory_swap_block = json_object();
    json_object_set(memory_swap_block, "size", json_integer(m->swap_size));
    json_object_set(memory_swap_block, "usage", json_integer(m->swap_usage));
    json_object_set(memory_block, "swap", memory_swap_block);

    json_object_set(json, "memory", memory_block);

    // Drives
    json_t *drives = json_array();
    for (drive *iter = m->drives; !iter->end; iter++) {
        json_t *drive = json_object();
        json_object_set(drive, "partition", json_string(iter->blockPath));
        json_object_set(drive, "mount_point", json_string(iter->mountPoint));
        json_object_set(drive, "size", json_integer(iter->size));
        json_object_set(drive, "usage", json_integer(iter->usage));
        json_array_append(drives, drive);
    }
    json_object_set(json, "drives", drives);

    return json;
}



