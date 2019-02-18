project(ejd_tests)

cmake_minimum_required (VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

function(build_test SRC_FILE)
	get_filename_component(src_name_we ${SRC_FILE} NAME_WE)
	set(target "${src_name_we}")
	add_executable(${target} ${SRC_FILE})
	# add_dependencies(${target} gtest) not needed now
	target_include_directories(
		${target}
		PRIVATE
		${CMAKE_SOURCE_DIR}/include
	)
	target_link_libraries(
		${target}
		PRIVATE
	)
	add_test(${target} ${target})
endfunction(build_test)

# Note : can split this into sub-sources for additional testing granulartiy
file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cxx")

foreach(test_src ${TEST_SOURCES})
	build_test(${test_src})
endforeach(test_src)
