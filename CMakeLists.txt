include(CMakePrintHelpers)

cmake_minimum_required(VERSION 3.11)
project("Red-Black-Tree")

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TESTS ${CMAKE_SOURCE_DIR}/tests)

set(RBTREE_EXE main_rbtree)
set(GEN_EXE gen_tests)
set(SET_EXE main_set)
set(BUILD_FOLDER build)

add_executable(${RBTREE_EXE} main_rbtree.cpp)
add_executable(${SET_EXE} main_set.cpp)
target_include_directories(${RBTREE_EXE} PUBLIC include)

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
