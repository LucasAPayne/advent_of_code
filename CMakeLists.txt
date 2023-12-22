cmake_minimum_required(VERSION 3.21)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(aoc_main)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(AOC_YEARS 2023)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(AocGenDays)

add_subdirectory(lib)

foreach (Y ${AOC_YEARS})
	message(${PROJECT_SOURCE_DIR}/src/${Y}/CMakeLists.txt)
	if(EXISTS ${PROJECT_SOURCE_DIR}/src/${Y}/CMakeLists.txt)
		message(Found ${Y})
		add_subdirectory(${PROJECT_SOURCE_DIR}/src/${Y})
	endif()
endforeach()
