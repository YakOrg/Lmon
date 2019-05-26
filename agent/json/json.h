/*
* Created by slagger on 5/11/19.
*/

#ifndef UNTITLED2_JSON_H
#define UNTITLED2_JSON_H

#include <jansson.h>
#include "../metrics/metrics.h"

json_t* make_json(metrics* m);

json_t *net_interfaces_json(network_interface *network_interfaces);

#endif
