//
// Created by dragon on 24.05.19.
//

#ifndef LMON_FETCH_H
#define LMON_FETCH_H

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#include "../agent.h"

char *fetch_data_from_agents(struct Agent *node);

#endif //LMON_FETCH_H
