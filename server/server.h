//
// Created by dragon on 10.05.19.
//

#ifndef LMON_SERVER_H
#define LMON_SERVER_H

#include <libbrd.h>

#include "../log/log.h"
#include "../http/http.h"
#include "../utils/utils.h"
#include "fetch/fetch.h"
#include "agent.h"

#define POSTBUFFERSIZE  512
#define MAXANSWERSIZE   512
#define POST            1

void start_server(int http_port);

typedef struct connection_info_struct {
    int connectiontype;
    char *answerstring;
    struct MHD_PostProcessor *postprocessor;
} connection_info;

#endif //LMON_SERVER_H
