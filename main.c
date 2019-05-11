#include "main.h"

int main(void) {
    metrics* m = malloc(sizeof(metrics));
    get_general_info(m);
    get_advanced_info(m);
    json_t* json = make_json(m);

    /*use it for debug
     * json_dump_file(json, "path/to/file, flags");
     * */
  	buf = json_dumps(json,0);
  	/*now buf contains the json of host*/
    return 0;
}