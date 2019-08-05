cmake_minimum_required(VERSION 3.13)
project(lmon C)
set(CMAKE_C_STANDARD 99)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-Wall -Os -O3 -static -pthread -Wl,--whole-archive -lpthread  -Wl,--no-whole-archive -DLOG_USE_COLOR")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

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

find_library(libsysmetrics libsysmetrics.a)
target_link_libraries(lmon ${libsysmetrics})