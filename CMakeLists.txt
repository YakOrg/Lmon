cmake_minimum_required(VERSION 3.13)
project(lmon C)
find_package(Threads)
set(CMAKE_C_STANDARD 99)

add_executable(lmon main.c http/http.c http/http.h agent/metrics/metrics.c agent/metrics/metrics.h agent/templates.h agent/agent.c agent/agent.h server/discovery.c server/discovery.h)
target_link_libraries(lmon ${CMAKE_THREAD_LIBS_INIT})