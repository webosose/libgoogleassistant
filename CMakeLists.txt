# @@@LICENSE
#
# Copyright (c) 2018 LG Electronics, Inc.
#
# LICENSE@@@

cmake_minimum_required(VERSION 2.8.7)
project(googleAssistant)
set(CMAKE_VERBOSE_MAKEFILE ON)

# setup for linking necessary libraries
include(FindPkgConfig)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JSONC REQUIRED json-c)
pkg_check_modules(PMLOGLIB REQUIRED PmLogLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3 -fpermissive ${GLIB2_CFLAGS_OTHER} ${JSONC_CFLAGS_OTHER} ${PMLOGLIB_CFLAGS_OTHER}")

# add include files
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/util
    ${GLIB2_INCLUDE_DIRS}
    ${JSONC_INCLUDE_DIRS}
    ${PMLOGLIB_INCLUDE_DIRS}
)

# link directories
link_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

# link snowboy-detect static library for keyword activation feature
add_library(snowboy-detect STATIC IMPORTED)
set_property(TARGET snowboy-detect PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libsnowboy-detect.a)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(SRC_LIST
    ${SRC_DIR}/util/common.cpp
    ${SRC_DIR}/util/logging.cpp
    ${SRC_DIR}/audioCapture.cpp
    ${SRC_DIR}/audioPlayback.cpp
    ${SRC_DIR}/googleAssistant.cpp
    ${SRC_DIR}/keywordDetector.cpp
    ${SRC_DIR}/speechRecognitionWorker.cpp
)

# add google apis
file(GLOB_RECURSE API ${GOOGLEAPIS_PATH}/api/*.cc)
file(GLOB_RECURSE RPC ${GOOGLEAPIS_PATH}/rpc/*.cc)
file(GLOB_RECURSE TYPE ${GOOGLEAPIS_PATH}/type/*.cc)
file(GLOB_RECURSE LOGGING ${GOOGLEAPIS_PATH}/logging/*.cc)
file(GLOB_RECURSE LONGRUNNING ${GOOGLEAPIS_PATH}/longrunning/*.cc)
file(GLOB_RECURSE ASSISTANT ${GOOGLEAPIS_PATH}/assistant/embedded/v1alpha2/*.cc)

# create sample excutable binary
set(BIN_NAME googleAssistant_sample)
add_executable(
    ${BIN_NAME}
    ${SRC_LIST}
    ${API}
    ${RPC}
    ${TYPE}
    ${LOGGING}
    ${LONGRUNNING}
    ${ASSISTANT}
    ${SRC_DIR}/main.cpp
)

target_link_libraries(
    ${BIN_NAME}
    grpc++
    pthread
    protobuf
    pulse
    pulse-simple
    snowboy-detect
    tatlas
    ${GLIB2_LDFLAGS}
    ${JSONC_LDFLAGS}
    ${PMLOGLIB_LDFLAGS}
)

# create shared library object
set(LIB_NAME googleassistant)
add_library(
    ${LIB_NAME} SHARED
    ${SRC_LIST}
    ${API}
    ${RPC}
    ${TYPE}
    ${LOGGING}
    ${LONGRUNNING}
    ${ASSISTANT}
)

target_link_libraries(
    ${LIB_NAME}
    grpc++
    pthread
    protobuf
    pulse
    pulse-simple
    snowboy-detect
    tatlas
    ${GLIB2_LDFLAGS}
    ${JSONC_LDFLAGS}
    ${PMLOGLIB_LDFLAGS}
)

set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
)

configure_file(${CMAKE_SOURCE_DIR}/googleassistant.pc.in googleassistant.pc @ONLY)

# install library
install(
    TARGETS ${LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    PROGRAMS ${CMAKE_SOURCE_DIR}/lib/libtatlas.so
    DESTINATION /usr/local/atlas/lib
)

file(GLOB_RECURSE PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
install(
    FILES ${PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
    FILES 
    ${CMAKE_SOURCE_DIR}/resources/keywordActivation/common.res
    ${CMAKE_SOURCE_DIR}/resources/keywordActivation/snowboy.umdl
    DESTINATION /etc/${PROJECT_NAME}
)

install(FILES ${CMAKE_BINARY_DIR}/googleassistant.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
