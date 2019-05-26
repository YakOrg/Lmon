//
// Created by dragon on 24.05.19.
//

#include "fetch.h"
#include "../../main.h"

struct url_data {
    size_t size;
    char *data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char *tmp;

    data->size += (size * nmemb);

    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

    if (tmp) {
        data->data = tmp;
    } else {
        if (data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char *url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if (NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

char *fetch_data_from_agents(struct Agent *a) {
    char *response = malloc(sizeof(char) * 2);
    strcpy(response, "[");

    for (agent *iter = a->first; iter; iter = iter->next) {
        log_debug("fetch metrics from %s", iter->endpoint);
        char *agent_metrics = handle_url(iter->endpoint);
        if (agent_metrics) {
            int new_fetched_str_len =
                    strlen(response) + strlen(agent_metrics) + (iter->next ? 2 : 1) + 1;

            response = realloc(response, new_fetched_str_len * sizeof(char));
            strcat(response, agent_metrics);
            free(agent_metrics);

            if (iter->next) {
                strcat(response, ", ");
            } else {
                strcat(response, "]");
            }
        } else continue;
    }
    return response;
}