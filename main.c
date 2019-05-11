#include "main.h"

char* get_buf_for_json(json_t* json, size_t* size)
{
    *size = json_dumpb(json, NULL, 0, 0);
    if (size == 0)
        return -1;

    char *buf = malloc(*size);

    return buf;
}

int main(void) {
    metrics* m = malloc(sizeof(metrics));
    get_general_info(m);
    get_advanced_info(m);
    json_t* json = make_json(m);

    buf = get_buf_for_json(json, &size);
    json_dumpb(json, buf, size, JSON_INDENT(JSON_MAX_INDENT));
    /*now json variable contents the json of host*/
    /*use it for debug
     * json_dump_file(json, "path/to/file, flags");
     * */
    return 0;
}