project(numerical_tests)

cmake_minimum_required (VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)

set(test_monotonestruct_src test_monotonestruct.cxx)
set(test_messages_src test_messages.cxx)
set(test_empdistr_src test_empdistr.cxx)
set(test_options_src test_options.cxx)
set(test_lobster_src test_lobster.cxx)
set(test_bidaskable_src test_bidaskable.cxx)

add_executable(test_monotonestruct ${test_monotonestruct_src} )
add_executable(test_empdistr ${test_empdistr_src})
add_executable(test_options ${test_options_src})

include_directories(
	test_monotonestruct
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

include_directories(
	test_empdistr
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

include_directories(
	test_options
	PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
	test_monotonestruct
	PUBLIC Mjolnir
)

target_link_libraries(
	test_empdistr
	PUBLIC Mjolnir
)

target_link_libraries(
	test_options
	PUBLIC Mjolnir
)

add_test(monotonestruct test_monotonestruct)
add_test(empdistr test_empdistr)
add_test(options test_options)

add_subdirectory(mpp)
