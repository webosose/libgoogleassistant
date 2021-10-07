# @@@LICENSE
#
# Copyright (c) 2018-2021 LG Electronics, Inc.
#
# LICENSE@@@

cmake_minimum_required(VERSION 2.8.7)
project(googleAssistant)
set(CMAKE_VERBOSE_MAKEFILE ON)

# setup for linking necessary libraries
include(FindPkgConfig)
pkg_check_modules(SNOWBOY REQUIRED snowboy)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JSONC REQUIRED json-c)
pkg_check_modules(PMLOGLIB REQUIRED PmLogLib)
pkg_check_modules(CBLAS REQUIRED cblas)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -O3 -fpermissive ${GLIB2_CFLAGS_OTHER} ${JSONC_CFLAGS_OTHER} ${PMLOGLIB_CFLAGS_OTHER} ${CBLAS_CFLAGS_OTHER}")

# add include files
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/util
    ${SNOWBOY_INCLUDE_DIRS}
    ${GLIB2_INCLUDE_DIRS}
    ${JSONC_INCLUDE_DIRS}
)

# link snowboy-detect static library for keyword activation feature
add_library(snowboy-detect STATIC IMPORTED)
set_property(TARGET snowboy-detect PROPERTY IMPORTED_LOCATION ${SNOWBOY_LIBRARY_DIRS}/libsnowboy-detect.a)

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
# keywordDetector.cpp links snowboy-detect.a so it needs snowboy_cflags_other eg. -std=c++11 or _GLIBCXX_USE_CXX11_ABI=0
set_property(SOURCE ${SRC_DIR}/keywordDetector.cpp PROPERTY COMPILE_FLAGS ${SNOWBOY_CFLAGS_OTHER})

# add google apis
file(GLOB_RECURSE API ${GOOGLEAPIS_PATH}/api/*.cc)
file(GLOB_RECURSE RPC ${GOOGLEAPIS_PATH}/rpc/*.cc)
file(GLOB_RECURSE TYPE ${GOOGLEAPIS_PATH}/type/*.cc)
file(GLOB_RECURSE LOGGING ${GOOGLEAPIS_PATH}/logging/*.cc)
file(GLOB_RECURSE LONGRUNNING ${GOOGLEAPIS_PATH}/longrunning/*.cc)
file(GLOB_RECURSE ASSISTANT ${GOOGLEAPIS_PATH}/assistant/embedded/v1alpha2/*.cc)

# create sample excutable binary
option (BUILD_SAMPLE "Build the sample executable" OFF)
if(BUILD_SAMPLE)
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
        ${GLIB2_LDFLAGS}
        ${JSONC_LDFLAGS}
        ${PMLOGLIB_LDFLAGS}
        ${CBLAS_LDFLAGS}
    )
endif()

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
    ${GLIB2_LDFLAGS}
    ${JSONC_LDFLAGS}
    ${PMLOGLIB_LDFLAGS}
    ${CBLAS_LDFLAGS}
)

set_target_properties(
    ${LIB_NAME}
    PROPERTIES
    VERSION 1.0.1
    SOVERSION 1
)

configure_file(${CMAKE_SOURCE_DIR}/googleassistant.pc.in googleassistant.pc @ONLY)

# Install target library
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Install public headers
file(GLOB_RECURSE PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
install(FILES ${PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Install google assistant configuration file
install(FILES ${CMAKE_SOURCE_DIR}/files/systemd/googleassistant.conf DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/systemd/system.conf.d)

# Install tools
file(GLOB_RECURSE TOOLS_JSON ${CMAKE_SOURCE_DIR}/tools/*.json)
install(FILES ${TOOLS_JSON} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/googleAssistant)

file(GLOB_RECURSE TOOLS_SCRIPT ${CMAKE_SOURCE_DIR}/tools/*.sh)
install(PROGRAMS ${TOOLS_SCRIPT} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/googleAssistant)

# Install pkgconfig file
install(FILES ${CMAKE_BINARY_DIR}/googleassistant.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
