cmake_minimum_required(VERSION 3.13)
project(lmon C)
set(CMAKE_C_STANDARD 99)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-Wall -Os -O3 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -DLOG_USE_COLOR")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_executable(lmon main.c agent/json/json.c agent/json/json.h main.h http/http.c http/http.h agent/metrics/metrics.c agent/metrics/metrics.h agent/agent.c agent/agent.h server/discovery.c server/discovery.h log/log.c log/log.h)

find_package(Threads)
target_link_libraries(lmon ${CMAKE_THREAD_LIBS_INIT})

find_library(JANSSON jansson)
target_link_libraries(lmon ${JANSSON})

include(FindPkgConfig)
find_package(LibHttp REQUIRED)
include_directories(${LIBHTTP_INCLUDE_DIR})
target_link_libraries(lmon ${LIBHTTP_LIBRARY} ${CMAKE_DL_LIBS})