cmake_minimum_required(VERSION 3.12)
project(mmd)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
add_compile_options(-Wall)

set(LINK_FLAGS "-lgdi32 -lopengl32")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -Wall -O0 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast")

set(CMAKE_SHARED_LINKER_FLAGS "${LINK_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "${LINK_FLAGS}")

include_directories(./src/include)
aux_source_directory(./src SrcFiles)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SrcFiles} resources.rc)