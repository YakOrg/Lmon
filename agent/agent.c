//
// Created by dragon on 10.05.19.
//

#include "agent.h"

char *genPage() {
    metrics m;
    get_base_metrics(&m);
    get_advanced_metrics(&m);

    json_t *json = make_json(&m);
    return json_dumps(json, JSON_REAL_PRECISION(3));
}

void startAgent(int httpPort) {
    startHttpServer(genPage, httpPort);
}