# This file is part of EJD.

# Copyright © 2019
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

project (EJD)

cmake_minimum_required (VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")

find_package(
	Boost 1.67 REQUIRED COMPONENTS
	filesystem
)

find_package(Eigen3 REQUIRED)
find_package(fmt REQUIRED)

add_library(
	EJD 
	SHARED
	src/EmpiricalDistribution.cxx
	src/ExtremeMeasures.cxx
)

target_include_directories(
	EJD
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	# PUBLIC "/usr/local/include/boost"
	PUBLIC ${Boost_INCLUDE_DIRS}
	PUBLIC ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(
	EJD
	fmt::fmt
	${Boost_LIBRARIES}
)

option(EJD_TESTS "Build thes tests" ON)
if(EJD_TESTS)
enable_testing()
add_subdirectory(tests)	
endif()

install (TARGETS EJD DESTINATION bin COMPONENT EJD)
