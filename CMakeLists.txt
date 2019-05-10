cmake_minimum_required(VERSION 3.13)
project(lmon C)

set(CMAKE_C_STANDARD 99)

add_executable(lmon main.c http.c http.h metrics.c metrics.h templates.h)