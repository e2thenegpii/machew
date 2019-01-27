cmake_minimum_required(VERSION 2.8.9)
project (machew)

option(build-toolchain "build-toolchain" OFF)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
