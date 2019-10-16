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

project(ejd_tests)

cmake_minimum_required (VERSION 3.11)

find_package(Eigen3 REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/googletest "extern/googletest")

function(build_test SRC_FILE)
	get_filename_component(src_name_we ${SRC_FILE} NAME_WE)
	set(target "${src_name_we}")
	add_executable(${target} ${SRC_FILE})
	# add_dependencies(${target} gtest) not needed now
	target_include_directories(
		${target}
		PRIVATE
		${CMAKE_SOURCE_DIR}/include
		${EIGEN3_INCLUDE_DIRS}
	)
	target_link_libraries(
		${target}
		PRIVATE
		EJD
		gtest
		gmock
		gtest_main
		fmt::fmt
		${Boost_LIBRARIES}
		discreture::discreture
	)
	target_compile_options(
		${target}
		PRIVATE "-Wall"
	)
	add_test(${target} ${target})
endfunction(build_test)

# Note : can split this into sub-sources for additional testing granulartiy
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/single_src_tests/*.cxx")

foreach(test_src ${TEST_SOURCES})
	build_test(${test_src})
endforeach(test_src)
