cmake_minimum_required(VERSION 3.0)

project("echo-linear_algebra")

enable_testing()

include(Echo/Test)

find_package(Echo)
find_package(EchoExecutionContext REQUIRED)
include(Echo/Default)

include_directories(SYSTEM ${ECHO_INTEL_EXECUTION_CONTEXT_INCLUDES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
${ECHO_INTEL_EXECUTION_CONTEXT_CXX_FLAGS}")

ECHO_ADD_DEFAULT_TEST()
ECHO_ADD_DEFAULT_BENCHMARK()
ECHO_INSTALL_DEFAULT_HEADERS()

ECHO_TARGET_LINK_LIBRARIES(${ECHO_INTEL_EXECUTION_CONTEXT_LIBRARIES})
