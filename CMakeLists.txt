cmake_minimum_required(VERSION 3.16)
set(CMAKE_SUPPRESS_REGENERATION true)

set(PROJECT_NAME TinyAlgoDemo)

set (CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME} CXX)

include(CMake/CMakeMacros.txt)

include_directories(.)
include_directories(TinyAlgo/)

add_subdirectory(tinyalgo ${CMAKE_CURRENT_BINARY_DIR}/TinyAlgo)

if(NOT EXTERNAL)
	add_subdirectory(examples/Library ${CMAKE_CURRENT_BINARY_DIR}/examples/Library)
	target_link_libraries(Library PRIVATE TinyAlgo)

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Library)
endif()