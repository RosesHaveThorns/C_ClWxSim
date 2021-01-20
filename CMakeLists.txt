cmake_minimum_required(VERSION 3.17)
project(C_ClWxSim C)

set(CMAKE_C_STANDARD 99)

include_directories(include/core/)
include_directories(include/sim/)
include_directories(include/ui/)
include_directories(include/utils/)

set(SOURCES
        src/sim/world.c
        src/ui/graph.c
        src/ui/ui.c
        src/utils/array_utils.c
        src/utils/ui_utils.c
        )


add_executable(C_ClWxSim ${SOURCES})