cmake_minimum_required(VERSION 3.1)
enable_testing()

find_package(   vivado_hls_cmake_helper
                HINTS $ENV{VIVADO_CMAKE_HELPER}
                REQUIRED)
project(ics_if_tx
    VERSION 0.1
    LANGUAGES NONE)

set(VENDOR "Akira_Nishiyama")
#set(VERSION "0.1")
set(LIBRARY_NAME "hls")
set(TARGET_DEVICE "xczu3eg-sbva484-1-e")
set(CLOCK_PERIOD "10")
#CFLAGS
set(CFLAGS "")
list(APPEND CFLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND CFLAGS "-I${GTEST_ROOT}/include")
set(CFLAGS_TB "${CFLAGS}")

#description
set(DESCRIPTION "Serializer of ICS_IF IP")

set(DISPLAY_IP_NAME "ICS_IF_TX")

#source list
set(SRC_FILES "")
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/ics_if_tx.cpp")

#testbench list
set(TESTBENCH_FILES "")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_ics_if_tx.cpp")

#directives.tcl path
set(DIRECTIVES ${CMAKE_CURRENT_SOURCE_DIR}/directives.tcl)
set(LDFLAGS "")
list(APPEND LDFLAGS "-L${GTEST_ROOT}/lib")
list(APPEND LDFLAGS "-lgtest_main")
list(APPEND LDFLAGS "-lgtest")

include(${VIVADO_HLS_EXPORT_CMAKE})

if(PRINT_VARIABLES)
    message(STATUS "*** dump start cmake variables ***")
    get_cmake_property(_variableNames VARIABLES)
    foreach(_variableName ${_variableNames})
            message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "*** dump end ***")
endif()
