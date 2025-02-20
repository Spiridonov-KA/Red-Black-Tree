include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.11)
project("Red-Black-Tree")

enable_testing()

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TESTS ${CMAKE_SOURCE_DIR}/tests)

set(RBTREE_EXE main_rbtree)
set(GEN_EXE gen_tests)
set(SET_EXE main_set)
set(TEST_EXE rbtree_test)
set(BUILD_FOLDER build)

add_executable(${RBTREE_EXE} main_rbtree.cpp)
add_executable(${SET_EXE} main_set.cpp)
add_executable(${TEST_EXE} test_file.cpp)
target_include_directories(${RBTREE_EXE} PUBLIC include)
target_include_directories(${TEST_EXE} PUBLIC include)
target_link_libraries(${RBTREE_EXE} gtest::gtest)
target_link_libraries(${TEST_EXE} GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${TEST_EXE})

add_executable(${GEN_EXE} gen_tests.cpp)


function(run_tests path_to_tests exe_file)
    file(GLOB testfiles "${path_to_tests}/*.dat")
    foreach(file ${testfiles})
        add_test(
                NAME ${file}.${exe_file}
                COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./${exe_file}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
			# set_tests_properties(${file} PROPERTIES DEPENDS ${exe_file})
    endforeach()
endfunction()

run_tests("tests" ${RBTREE_EXE})
run_tests("tests" ${SET_EXE})
