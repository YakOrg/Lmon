//
// Created by dragon on 10.05.19.
//

#ifndef LMON_AGENT_H
#define LMON_AGENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <curl/curl.h>
#include <microhttpd.h>

#include <sysmetrics.h>
#include <libbrd.h>

#include "../http/http.h"
#include "json/json.h"
#include "../utils/utils.h"
#include "../main.h"

void start_agent(int http_port, char *server_url);

#endif //LMON_AGENT_H
