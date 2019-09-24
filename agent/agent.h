#ifndef LMON_AGENT_H
#define LMON_AGENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <curl/curl.h>
#include <microhttpd.h>

#include <sysmetrics.h>
#include <libbrd.h>

#include "json/json.h"
#include "../http/http.h"
#include "../utils/utils.h"
#include "../main.h"

void start_agent(int http_port);

#endif
