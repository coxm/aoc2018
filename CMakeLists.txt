cmake_minimum_required(VERSION 3.5)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


project(aoc LANGUAGES CXX VERSION 0.0.0)


if(NOT INPUTS_DIR)
	set(INPUTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inputs/")
endif()
message(STATUS "Using inputs dir '${INPUTS_DIR}'")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/settings.h"
	"${CMAKE_CURRENT_BINARY_DIR}/settings.h")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# Compiler flags.
list(APPEND MAIN_CXX_FLAGS
	-Werror -std=c++1z -Wall -Wold-style-cast -Woverloaded-virtual
	-Wsign-promo -Wno-pmf-conversions -Wshadow
)
if(CMAKE_COMPILER_IS_GNUCXX)
	list(APPEND MAIN_CXX_FLAGS -Weffc++)
endif()
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	list(APPEND MAIN_CXX_FLAGS -O0 -ggdb3)
else()
	list(APPEND MAIN_CXX_FLAGS -O3)
endif()


function(add_day DAY)
	set(BIN_NAME "day-${DAY}")
	add_executable(${BIN_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/${DAY}/main.cpp")
	target_compile_options(${BIN_NAME} PRIVATE ${MAIN_CXX_FLAGS})
endfunction()


add_day(01)
add_day(02)
add_day(03)
add_day(04)
add_day(05)
add_day(06)
add_day(07)
add_day(08)
add_day(09)
