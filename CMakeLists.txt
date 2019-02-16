project (EJD)
# enable_testing()

cmake_minimum_required (VERSION 3.12)

# setting CXX standard properties GLOBALLY
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
# set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
# bad practice
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# need the below for MKL
set(CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")

find_package(
	Boost 1.67 REQUIRED COMPONENTS
	filesystem
)

find_package(Eigen3 REQUIRED)
# include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

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

# numerical tests
# TO-DO : should generalize later to different type of tests
# add_subdirectory(tests)	

install (TARGETS EJD DESTINATION bin COMPONENT EJD)
