cmake_minimum_required (VERSION 3.0)
project (NoTeX)

set(CMAKE_CXX_STANDARD 14)

set(NoTeX_VERSION_MAJOR 1)
set(NoTeX_VERSION_MINOR 0)

configure_file(
    "${PROJECT_SOURCE_DIR}/CompileConfig.h.in"
    "${PROJECT_BINARY_DIR}/CompileConfig.h"
)

option(DEBUG "Print debug output and statistics" ON)

if(DEBUG)
	add_definitions(-DDEBUG)
endif(DEBUG)

#FIND_PACKAGE(PkgConfig REQUIRED)

# FIND GTK
#pkg_check_modules(GTK gtkmm-3.0)

# Setup NoTeX executable
add_executable(${PROJECT_NAME} "")

# Make sure to include the libs and dirs

add_subdirectory(src)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(${PROJECT_NAME} Source)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_BINARY_DIR}"
)
