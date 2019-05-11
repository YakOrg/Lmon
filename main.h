/*
* Created by slagger on 5/11/19.
*/

#ifndef UNTITLED2_MAIN_H
#define UNTITLED2_MAIN_H

#include "metrics.h"
#include "json.h"

char* get_buf_for_json(json_t* json, size_t* size);

size_t size;
char* buf;

#endif
