cmake_minimum_required(VERSION 3.13)
project(lmon C)
set(CMAKE_C_STANDARD 99)

find_library(JANSSON jansson)
find_package(Threads)

add_executable(lmon main.c metrics.c metrics.h json.c json.h main.h http/http.c http/http.h agent/metrics/metrics.c agent/metrics/metrics.h agent/templates.h agent/agent.c agent/agent.h server/discovery.c server/discovery.h)
target_link_libraries(lmon ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(lmon ${JANSSON})