//
// Created by dragon on 10.05.19.
//

#include "server.h"

int contains(agent *agents, char *endpoint) {
    if (!agents) return 0;
    for (agent *iter = agents->first; iter; iter = iter->next)
        if (!strcmp(iter->endpoint, endpoint))
            return 1;
    return 0;
}

char *add_agent(agent **agents_pointer_to_pointer, char *post_data) {
    agent *agents = *agents_pointer_to_pointer;
    char *resp = malloc(MAXANSWERSIZE);
    if (contains(agents, post_data)) {
        log_trace("agent present in list (%s)", post_data);
        sprintf(resp, "\"PRESENT\"");
    } else {
        agent *agent = malloc(sizeof(struct Agent));
        char *endpoint = malloc(sizeof(char) * 256);
        sprintf(endpoint, "%s", post_data);
        agent->endpoint = endpoint;
        agent->next = NULL;
        if (!agents) {
            agent->first = agent;
        } else {
            agent->first = agents->first;
            agents->next = agent;
        }
        *agents_pointer_to_pointer = agent;
        sprintf(resp, "\"OK\"");
        log_info("added agent (%s)", post_data);
    }
    return resp;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

static int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                        const char *filename, const char *content_type,
                        const char *transfer_encoding, const char *data, uint64_t off,
                        size_t size) {
    struct connection_info_struct *con_info = coninfo_cls;
    if (0 == strcmp(key, "endpoint")) {
        if (size > 0) {
            char *answerstring;
            answerstring = malloc(MAXANSWERSIZE);
            if (!answerstring) return MHD_NO;
            snprintf(answerstring, MAXANSWERSIZE, "%s", data);
            con_info->answerstring = answerstring;
        } else
            con_info->answerstring = NULL;

        return MHD_NO;
    }

    return MHD_YES;
}

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

static void request_completed(void *cls, struct MHD_Connection *connection,
                              void **con_cls, enum MHD_RequestTerminationCode toe) {
    struct connection_info_struct *con_info = *con_cls;

    if (NULL == con_info)
        return;

    if (con_info->connectiontype == POST) {
        MHD_destroy_post_processor(con_info->postprocessor);
        if (con_info->answerstring)
            free(con_info->answerstring);
    }

    free(con_info);
    *con_cls = NULL;
}

#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

static int handler(void *cls,
                   struct MHD_Connection *connection,
                   const char *url,
                   const char *method,
                   const char *version,
                   const char *upload_data,
                   size_t *upload_data_size,
                   void **con_cls) {

    int status;
    agent **agents_pointer_to_pointer = (agent **) cls;
    agent *agents = *agents_pointer_to_pointer;

    if (!strcmp(method, "GET") && !strcmp(method, "POST"))
        return MHD_NO;
    else if (!strcmp(url, "/"))
        status = send_json(connection, fetch_data_from_agents(agents));
    else if (!strcmp(url, "/agents/add") && !strcmp(method, "POST")) {
        if (!*con_cls) {
            struct connection_info_struct *con_info;
            con_info = malloc(sizeof(struct connection_info_struct));
            if (!con_info) return MHD_NO;
            con_info->answerstring = NULL;
            con_info->postprocessor =
                    MHD_create_post_processor(connection, POSTBUFFERSIZE,
                                              iterate_post, (void *) con_info);

            if (NULL == con_info->postprocessor) {
                free(con_info);
                return MHD_NO;
            }

            con_info->connectiontype = POST;
            *con_cls = (void *) con_info;
            return MHD_YES;
        }

        struct connection_info_struct *con_info = *con_cls;

        if (*upload_data_size != 0) {
            MHD_post_process(con_info->postprocessor, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        } else if (con_info->answerstring) {
            status = send_json(connection,
                               add_agent(agents_pointer_to_pointer, con_info->answerstring));
        } else {
            status = bad_request(connection);
        }

    } else {
        status = not_found(connection);
    }

    return status;
}

#pragma clang diagnostic pop

void run_httpd(int port, agent **agent) {
    MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                     port,
                     NULL,
                     NULL,
                     &handler,
                     agent,
                     MHD_OPTION_NOTIFY_COMPLETED,
                     request_completed,
                     NULL,
                     MHD_OPTION_END);
}

void start_server(int http_port) {
    agent **agent_p = malloc(sizeof(agent *));
    *agent_p = NULL;

    run_httpd(http_port, agent_p);
    run_blocking_broadcast(itoa(http_port));
}
