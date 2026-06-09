cmake_minimum_required(VERSION 4.1)
project(rubikCube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto=auto -DNDEBUG -pthread -fopenmp")

add_subdirectory(../claLib ${CMAKE_BINARY_DIR}/claLib)

find_package(OpenGL REQUIRED)

add_library(rubikCubeLib INTERFACE)
target_include_directories(rubikCubeLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rubikCube rubikCube.cpp)

target_link_libraries(rubikCube PRIVATE claLib)
target_link_libraries(rubikCube PRIVATE OpenGL::GL)
target_link_libraries(rubikCube PRIVATE -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -fopenmp)
target_link_libraries(rubikCube PRIVATE rubikCubeLib)

target_compile_definitions(rubikCube PRIVATE GL_GLEXT_PROTOTYPES)
