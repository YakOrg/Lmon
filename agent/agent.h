//
// Created by dragon on 10.05.19.
//

#ifndef LMON_AGENT_H
#define LMON_AGENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../http/http.h"
#include "metrics/metrics.h"
#include "json/json.h"

void startAgent(int httpPort);

#endif //LMON_AGENT_H
