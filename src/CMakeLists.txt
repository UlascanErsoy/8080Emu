cmake_minimum_required(VERSION 2.7)

project(8080Emulator)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 2)

add_definitions("-Wall")

configure_file (
	"${PROJECT_SOURCE_DIR}/common.h.in"
	"${PROJECT_SOURCE_DIR}/common.h"
	)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(8080Emulator main.c emulator.c cpu.c memory.c timer.c)

