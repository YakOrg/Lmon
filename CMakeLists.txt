cmake_minimum_required(VERSION 3.13)
project(lmon C)

set(CMAKE_C_STANDARD 99)

add_executable(lmon main.c agent/http/http.c agent/http/http.h agent/metrics/metrics.c agent/metrics/metrics.h agent/http/templates.h agent/agent.c agent/agent.h server/discovery.c server/discovery.h)