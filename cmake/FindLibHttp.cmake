set(FIND_LIBHTTP_PATHS
        /usr/lib/libhttp)

find_path(LIBHTTP_INCLUDE_DIR libhttp.h
        PATH_SUFFIXES include
        PATHS ${FIND_LIBHTTP_PATHS})

find_library(LIBHTTP_LIBRARY
        NAMES libhttp
        PATH_SUFFIXES lib
        PATHS ${FIND_LIBHTTP_PATHS})