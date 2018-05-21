# @@@LICENSE
#
# Copyright (c) 2018 LG Electronics, Inc.
#
# LICENSE@@@

cmake_minimum_required(VERSION 2.8.7)
project(googleAssistant)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3 -fpermissive")

# add include files
include_directories(${CMAKE_SOURCE_DIR}/include)

# link directories
link_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

# link static library
add_library(snowboy-detect STATIC IMPORTED)
set_property(TARGET snowboy-detect PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libsnowboy-detect.a)

# create executable file
set(BIN_NAME googleAssistant)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SRC_LIST
    ${SRC_DIR}/audioCapture.cpp
    ${SRC_DIR}/keywordDetector.cpp
    ${SRC_DIR}/main.cpp
)

add_executable(
    ${BIN_NAME}
    ${SRC_LIST}
)

target_link_libraries(
    ${BIN_NAME}
    snowboy-detect
    pthread
    pulse
    pulse-simple
)
