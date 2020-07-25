cmake_minimum_required(VERSION 3.1)
enable_testing()


find_program(   VIVADO_COMMAND
                NAME vivado
                HINTS ENV var
                REQUIRED)

find_file(  HELPER_SCRIPT_IPX
            NAME create_ip.tcl
            HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(  HELPER_SCRIPT_PRJ_GEN
            NAME project_generation.tcl
            HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(  HELPER_SCRIPT_IMPORT_BD
            NAME vivado_import_bd.tcl
            HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(  HELPER_SCRIPT_EXPORT_SIM
            NAME vivado_export_sim.tcl
            HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(  HELPER_SCRIPT_OPEN_WDB
            NAME vivado_open_wdb.tcl
            HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(  VIVADO_IPX_EXPORT_CMAKE
            NAME vivado_ipx_export.cmake
            HINTS $ENV{VIVADO_CMAKE_HELPER}
            REQUIRED)

find_file(  VIVADO_ADD_SIM_CMAKE
            NAME vivado_add_sim.cmake
            HINTS $ENV{VIVADO_CMAKE_HELPER})

#version fixed to 1.0 due to vivado 2019.2 restriction.
project(ics_if
    VERSION 1.0
    LANGUAGES NONE)

set(CP "cp")
set(CP_OPTION "-rf")
set(MV "mv")
set(RM "rm")
set(RM_OPTION "-rf")
set(SED "sed")
set(MKDIR "mkdir")
set(MKDIR_OPTION "-p")

add_subdirectory(modules)

set(VENDOR "Akira_Nishiyama")
set(LIBRARY_NAME "user")
set(TARGET_DEVICE "xczu3eg-sbva484-1-e")

#description
set(DESCRIPTION "ICS_IF IP")
#string(REPLACE " " ":" DESCRIPTION "${DESCRIPTION}")

set(DISPLAY_IP_NAME "ICS_IF")

#source list
set(SRC_FILES "")
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/interval_timer.v")
string(REPLACE ";" ":" SRC_FILES "${SRC_FILES}")

#testbench list
set(TESTBENCH_FILES "")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_interval_timer.v")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_uvm_test.sv")
string(REPLACE ";" ":" TESTBENCH_FILES "${TESTBENCH_FILES}")

#ip_repo_path list
set(IP_REPO_PATH "")
list(APPEND IP_REPO_PATH "${CMAKE_CURRENT_BINARY_DIR}")
string(REPLACE ";" ":" IP_REPO_PATH "${IP_REPO_PATH}")

#vivado create_project name(currently fixed to project_1)
set(VIVADO_PRJ_NAME "project_1")
set(SIMSET_NAME "sim_1")

#create block design scripts
set(BLOCK_DESIGN_TCL "${CMAKE_CURRENT_SOURCE_DIR}/scripts/blockdesign.tcl")

include(${VIVADO_IPX_EXPORT_CMAKE})

add_dependencies( ${PROJECT_NAME} ics_if_main)
add_dependencies( ${PROJECT_NAME} ics_if_tx)
add_dependencies( ${PROJECT_NAME} ics_if_rx)

#simulation
set(DEPENDENCIES ${PROJECT_NAME})
set(SIMULATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/sim")
file(MAKE_DIRECTORY ${SIMULATION_DIR}/work/xsim)
add_custom_target(gen_all DEPENDS ${DEPENDENCIES})
add_custom_target(compile_all DEPENDS gen_all)
add_custom_target(elaborate_all DEPENDS compile_all)
add_custom_target(sim_all DEPENDS elaborate_all)

set(ADDITIONAL_VLOG_OPTS "-L uvm")
set(ADDITIONAL_ELAB_OPTS "-L uvm")

include(${VIVADO_ADD_SIM_CMAKE})

add_sim(tb_interval_timer ${SIMULATION_DIR} "${DEPENDENCIES};${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_interval_timer.v" ${ADDITIONAL_VLOG_OPTS} ${ADDITIONAL_ELAB_OPTS} "")
add_sim(tb_uvm_test ${SIMULATION_DIR} "${DEPENDENCIES};${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_uvm_test.sv" ${ADDITIONAL_VLOG_OPTS} ${ADDITIONAL_ELAB_OPTS} tb_interval_timer)

if(PRINT_VARIABLES)
    message(STATUS "*** dump start cmake variables ***")
    get_cmake_property(_variableNames VARIABLES)
    foreach(_variableName ${_variableNames})
            message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "*** dump end ***")
endif()
