#ifndef UNTITLED2_MAIN_H
#define UNTITLED2_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "log/log.h"

#include "agent/agent.h"
#include "server/server.h"

#define USAGE \
"NAME:\n"\
"   lmon - monitoring, but small and simple\n\n"\
"USAGE:\n"\
"   lmon [commands] [arguments...]\n\n"\
"COMMANDS:\n"\
"   server   Run management server\n"\
"   agent    Run node agent\n\n"\
"ARGS:\n"\
"   --http-port <PORT> http server port\n"\
"   --help             show this help\n"\
"   --daemon           start agent as daemon\n"\
"   --debug            enable debug log messages\n"\

#endif
