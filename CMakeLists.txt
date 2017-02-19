cmake_minimum_required (VERSION 2.6)
project (PathNormalization)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src)

add_library(fs_normalize STATIC src/path_normalize.cpp)

add_executable(normalize src/normalize.cpp)
target_link_libraries(normalize fs_normalize)

# tests
add_executable(unit tests/unit.cpp)
target_link_libraries(unit fs_normalize)

add_executable(perf tests/perf.cpp)
target_link_libraries(perf fs_normalize)

set(NORMALIZE_DESTINATION_BIN bin)
install (TARGETS normalize DESTINATION ${NORMALIZE_DESTINATION_BIN})
install (TARGETS unit DESTINATION ${NORMALIZE_DESTINATION_BIN})
install (TARGETS perf DESTINATION ${NORMALIZE_DESTINATION_BIN})

################################################################################
# Performance Tests

set(SECONDS_PER_TEST 30)

add_custom_target(perf_test DEPENDS perf
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 10 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 100 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 1000 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 10000 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 100000 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 1000000 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 10000000 ${SECONDS_PER_TEST}
  COMMAND ${NORMALIZE_DESTINATION_BIN}/perf 100000000 ${SECONDS_PER_TEST}
)

################################################################################
# Unit Tests
include(CTest)
set (CTEST_PROJECT_NAME "PathNormalize")

add_test (Y/AAA/../CCC/./DDD/../EEE/../FFF/./../.. ${NORMALIZE_DESTINATION_BIN}/unit Y/AAA/../CCC/./DDD/../EEE/../FFF/./../.. Y/)
add_test (/ ${NORMALIZE_DESTINATION_BIN}/unit / /)
add_test (// ${NORMALIZE_DESTINATION_BIN}/unit // /)
add_test (/// ${NORMALIZE_DESTINATION_BIN}/unit /// /)
add_test (f ${NORMALIZE_DESTINATION_BIN}/unit f f/)
add_test (foo ${NORMALIZE_DESTINATION_BIN}/unit foo foo/)
add_test (foo/. ${NORMALIZE_DESTINATION_BIN}/unit foo/. foo/)
add_test (/foo ${NORMALIZE_DESTINATION_BIN}/unit /foo /foo)
add_test (/foo/ ${NORMALIZE_DESTINATION_BIN}/unit /foo/ /foo)
add_test (/foo/. ${NORMALIZE_DESTINATION_BIN}/unit /foo/. /foo)
add_test (/foo/bar ${NORMALIZE_DESTINATION_BIN}/unit /foo/bar /foo/bar)
add_test (.. ${NORMALIZE_DESTINATION_BIN}/unit .. ../)
add_test (../.. ${NORMALIZE_DESTINATION_BIN}/unit ../.. ../)
add_test (../../. ${NORMALIZE_DESTINATION_BIN}/unit ../../. ../)
add_test (/.. ${NORMALIZE_DESTINATION_BIN}/unit /.. /)
add_test (/../.. ${NORMALIZE_DESTINATION_BIN}/unit /../.. /)
add_test (../foo ${NORMALIZE_DESTINATION_BIN}/unit ../foo ../foo)
add_test (. ${NORMALIZE_DESTINATION_BIN}/unit . ./)
add_test (./. ${NORMALIZE_DESTINATION_BIN}/unit ./. ./)
add_test (/foo/.. ${NORMALIZE_DESTINATION_BIN}/unit /foo/.. /)
add_test (/foo/... ${NORMALIZE_DESTINATION_BIN}/unit /foo/... /foo/...)
add_test (/foo/.../. ${NORMALIZE_DESTINATION_BIN}/unit /foo/.../. /foo/...)
add_test (/foo/..bar ${NORMALIZE_DESTINATION_BIN}/unit /foo/..bar /foo/..bar)
add_test (../f ${NORMALIZE_DESTINATION_BIN}/unit ../f ../f)
add_test (/../f ${NORMALIZE_DESTINATION_BIN}/unit /../f /f)
add_test (/net/foo/bar ${NORMALIZE_DESTINATION_BIN}/unit /net/foo/bar /net/foo/bar)
add_test (/net/foo/bar/. ${NORMALIZE_DESTINATION_BIN}/unit /net/foo/bar/. /net/foo/bar)
add_test (net/foo/bar ${NORMALIZE_DESTINATION_BIN}/unit net/foo/bar/ net/foo/bar)

# The 'check' target is used to perform a build and then run tests.  This
# approach is done in lieu of a way to make the CMake generated 'test' target
# depend on 'all'.
add_custom_target(
  check
  COMMAND ${CMAKE_COMMAND}
    --build ${CMAKE_BINARY_DIR}
    --target all
  COMMAND ${CMAKE_CTEST_COMMAND}
  COMMAND ${CMAKE_COMMAND}
    --build ${CMAKE_BINARY_DIR}
    --target perf_test
)
