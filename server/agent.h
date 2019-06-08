//
// Created by dragon on 26.05.19.
//

#ifndef LMON_AGENT_STRUCT_H
#define LMON_AGENT_STRUCT_H

typedef struct Agent {
    char*  endpoint;
    struct Agent *next;
    struct Agent *first;
} agent;

#endif //LMON_AGENT_H
