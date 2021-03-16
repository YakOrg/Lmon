cmake_minimum_required(VERSION 3.9)
project(lmon C)

set(CMAKE_C_STANDARD 99)

SET(CMAKE_EXE_LINKER_FLAGS "-Wall -Os -O3")

add_executable(lmon main.c agent/json/json.c agent/json/json.h main.h http/http.h agent/agent.c agent/agent.h server/server.c server/server.h log/log.c log/log.h server/agent.h server/fetch/fetch.c server/fetch/fetch.h http/http.c utils/utils.c utils/utils.h)

find_package(Threads)
target_link_libraries(lmon ${CMAKE_THREAD_LIBS_INIT})

find_library(libcurl curl)
target_link_libraries(lmon ${libcurl})

find_library(JANSSON jansson)
target_link_libraries(lmon ${JANSSON})

find_library(libhttp microhttpd)
target_link_libraries(lmon ${libhttp})

find_library(libbrd libbrd.a)
target_link_libraries(lmon ${libbrd})

find_library(libmetrics libsysmetrics.a)
target_link_libraries(lmon ${libmetrics})