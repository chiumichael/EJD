project (EJD)
enable_testing()

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

add_library(
	EJD SHARED
	# utils
	src/EmpiricalDistribution.cxx
	src/ExtremeMeasures.cxx
)

target_include_directories(
	EJD
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PUBLIC "/usr/local/include/"
	PUBLIC "/usr/local/include/boost"
	PUBLIC ${EIGEN3_INCLUDE_DIR}
)

target_link_libraries(
	EJD
	"/usr/local/lib/libfmt.a"
	${Boost_LIBRARIES}
	${Boost_FILESYSTEM_LIBRARY}
)

add_subdirectory(tests)	

install (TARGETS EJD DESTINATION bin COMPONENT EJD)
