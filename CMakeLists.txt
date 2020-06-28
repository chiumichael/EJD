# This file is part of EJD.

# Copyright © 2020
#           Michael Chiu <chiu@cs.toronto.edu>

# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

cmake_minimum_required (VERSION 3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.15)
	message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
	cmake_policy(VERSION 3.13)
endif()

##
## GLOBAL VARIABLES
##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

# set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")
include("${CMAKE_CURRENT_LIST_DIR}/project-meta-info.in")

project(EJD 
		VERSION ${project_version}
		DESCRIPTION ${project_description}
		HOMEPAGE_URL ${project_homepage}
		LANGUAGES C CXX)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_supported OUTPUT error)

##
## INCLUDE
##
find_package(Boost 1.65 REQUIRED COMPONENTS)
find_package(fmt REQUIRED)
find_package(blaze 3.6 REQUIRED)
find_package(nlohmann_json 3.7.0 REQUIRED)
find_package(range-v3 REQUIRED)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif()

include(GNUInstallDirs)

add_subdirectory(extern/discreture)

##
## OPTIONS
##
option(EJD_TESTS "Build tests" ON)
option(EJD_BENCHMARKS "Build benchmarks" ON)
option(USE_LD "Use LLD Linker" OFF)

add_library(EJD SHARED)

target_sources(
	EJD
	PRIVATE	src/AnsiColor.cxx
			src/Correlation.cxx
			src/EmpiricalDistribution.cxx
			src/ExtremeMeasures.cxx
)

target_include_directories(
	EJD
	PUBLIC	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE	${Boost_INCLUDE_DIRS}
)

target_link_libraries(
	EJD
	PRIVATE fmt::fmt
			discreture::discreture
			blaze::blaze
			range-v3::range-v3
)

# TODO: improve this
target_compile_options(
	EJD
	PRIVATE "-Wall"
)

if(LTO_supported)
	message("LTO Supported")
	set_target_properties(EJD PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(EJD_TESTS)
	find_package(HighFive 2.1 REQUIRED)
	target_link_libraries(
		EJD
		PRIVATE HighFive
	)
	enable_testing()
	add_subdirectory(tests)
endif()

if(EJD_BENCHMARKS)
	set(BENCHMARK_ENABLE_TESTING OFF)
	set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
	add_subdirectory(benchmarks)
endif()

install(TARGETS EJD)
