#include "json.h"

json_t *net_interfaces_json(network_interface *network_interfaces) {
    json_t *interfaces = json_array();
    for (network_interface *iter = network_interfaces; iter; iter = iter->next) {
        json_t *interface = json_object();
        json_object_set_new(interface, "name", json_string(iter->interface_name));

        json_t *addresses = json_object();
        json_t *ipv4_addresses = json_array();
        json_t *ipv6_addresses = json_array();
        for (net_address *address = iter->addresses; address; address = address->next)
            if (address->type == IPV4)
                json_array_append_new(ipv4_addresses, json_string(address->ip_address));
            else if (address->type == IPV6)
                json_array_append_new(ipv6_addresses, json_string(address->ip_address));

        if (!json_array_size(ipv4_addresses) && !json_array_size(ipv6_addresses)) {
            json_decref(ipv6_addresses);
            json_decref(ipv4_addresses);
            json_decref(addresses);
            json_decref(interface);
            continue;
        }

        json_object_set_new(addresses, "ipv4", ipv4_addresses);
        json_object_set_new(addresses, "ipv6", ipv6_addresses);
        json_object_set_new(interface, "addresses", addresses);
        json_array_append_new(interfaces, interface);
    }
    return interfaces;
}

json_t *make_json(metrics *m) {
    json_t *json = json_object();

    // Info block
    json_t *info_block = json_object();


    json_object_set_new(info_block, "hostname", json_string(m->hostname));
    json_object_set_new(info_block, "uptime", json_integer(m->uptime));

    json_t *info_sys_block = json_object();
    json_object_set_new(info_sys_block, "name", json_string(m->sys_name));
    json_object_set_new(info_sys_block, "release", json_string(m->sys_release));
    json_object_set_new(info_sys_block, "version", json_string(m->sys_version));
    json_object_set_new(info_sys_block, "arch", json_string(m->arch));
    json_object_set_new(info_block, "system", info_sys_block);

    json_object_set_new(json, "info", info_block);


    // CPU block
    json_t *cpu_block = json_object();
    json_object_set_new(cpu_block, "processors_count", json_integer(m->processors_count));
    json_object_set_new(cpu_block, "load_avg", json_real(m->load_average));
    json_object_set_new(cpu_block, "processes_count", json_integer(m->processes_count));
    json_object_set_new(json, "cpu", cpu_block);


    // Memory block
    json_t *memory_block = json_object();
    json_object_set_new(memory_block, "size", json_integer(m->ram_size));
    json_object_set_new(memory_block, "usage", json_integer(m->ram_usage));
    json_object_set_new(memory_block, "shared", json_integer(m->ram_shared));
    json_object_set_new(memory_block, "buffer", json_integer(m->ram_buffer));

    json_t *memory_swap_block = json_object();
    json_object_set_new(memory_swap_block, "size", json_integer(m->swap_size));
    json_object_set_new(memory_swap_block, "usage", json_integer(m->swap_usage));
    json_object_set_new(memory_block, "swap", memory_swap_block);

    json_object_set_new(json, "memory", memory_block);

    // Temperature block
    json_t *temperatures = json_object();
    for (temp_input *iter = m->temp_inputs; iter; iter = iter->next) {
        json_t *temp_device = json_object();
        json_object_set_new(temp_device, "current", json_integer(iter->current_value));
        json_object_set_new(temp_device, "max", json_integer(iter->max_value));
        json_object_set_new(temperatures, iter->device_name, temp_device);
    }
    json_object_set_new(json, "temperature", temperatures);

    // Network interfaces
    json_object_set_new(json, "interfaces", net_interfaces_json(m->network_interfaces));

    // Drives
    json_t *drives = json_array();
    for (drive *iter = m->drives; !iter->end; iter++) {
        json_t *drive = json_object();
        json_object_set_new(drive, "partition", json_string(iter->blockPath));
        json_object_set_new(drive, "mount_point", json_string(iter->mountPoint));
        json_object_set_new(drive, "size", json_integer(iter->size));
        json_object_set_new(drive, "usage", json_integer(iter->usage));
        json_array_append_new(drives, drive);
    }
    json_object_set_new(json, "drives", drives);

    return json;
}



