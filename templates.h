//
// Created by dragon on 10.05.19.
//

#ifndef LMON_TEMPLATES_H
#define LMON_TEMPLATES_H

#define BASE_TEMPLATE\
 "\"info\": {\"hostname\": \"%s\", \"kernel\": \"%s\"}, "\
 "\"cpu\": {\"load_avg\": %.2f}, "\
 "\"memory\": {\"size\": %d, \"usage\": %d, \"swap\": {\"size\": %d, \"usage\": %d}}"

#define objectStart "{"
#define objectEnd "}"


#define DRIVES_OBJ "\"drives\": "
#define delimiter ", "
#define arrayStart "["
#define arrayEnd "]"
#define DRIVE_TEMPLATE "{\"partition\": \"%s\", \"mountPoint\": \"%s\", \"size\": %d, \"usage\": %d}"

#endif //LMON_TEMPLATES_H
