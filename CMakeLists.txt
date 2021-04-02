cmake_minimum_required(VERSION 3.17)
project(C_ClWxSim C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

include_directories(include/core/)
include_directories(include/sim/)
include_directories(include/ui/)
include_directories(include/utils/)

set (PROJECT_LINK_LIBS libfftw3-3.dll)
link_directories (lib/fftw3)
include_directories (lib/fftw3)

set(SOURCES
        src/sim/world.c
        src/sim/pressure.c
        src/sim/controller.c
        src/ui/graph.c
        src/ui/ui.c
        src/utils/array_utils.c
        src/utils/ui_utils.c
        src/utils/maths_utils.c)


add_executable(C_ClWxSim ${SOURCES})

find_library(FFTW3 FFTW3 HINTS lib/*)
target_link_libraries(C_ClWxSim ${PROJECT_LINK_LIBS})