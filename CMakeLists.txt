cmake_minimum_required(VERSION 3.1)
enable_testing()

set(DOC_GEN ON)

find_package(   vivado_cmake_helper
                HINTS $ENV{VIVADO_CMAKE_HELPER}
                REQUIRED)

#version fixed to 1.0 due to vivado 2019.2 restriction.
project(ics_if
    VERSION 1.0
    LANGUAGES NONE)

add_subdirectory(modules)

set(VENDOR "Akira_Nishiyama")
set(LIBRARY_NAME "user")
set(TARGET_DEVICE "xczu3eg-sbva484-1-e")

#description
set(DESCRIPTION "ICS_IF IP")
#string(REPLACE " " ":" DESCRIPTION "${DESCRIPTION}")

set(DISPLAY_IP_NAME "ICS_IF")

require(simple_axi_uvm
        GIT_REPOSITORY https://github.com/akira-nishiyama/simple_axi_uvm.git
        )
require(simple_ics_uvm
        GIT_REPOSITORY https://github.com/akira-nishiyama/simple_ics_uvm.git
       )
require(gp_scoreboard
        GIT_REPOSITORY https://github.com/akira-nishiyama/uvm_component.git
       )

#source list
set(SRC_FILES "")
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/interval_timer.v")
list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/dff_with_we.v")

#testbench list
#file(GLOB UVM_AXI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/src*.sv)
set(TESTBENCH_FILES "")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_interval_timer.v")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_uvm_test.sv")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_ics_if_uvm.sv")
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_ics_if_uvm_test_pkg.sv")
#list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/lib/uvm_gp_scoreboard/gp_scoreboard.sv")
list(APPEND TESTBENCH_FILES "${gp_scoreboard_SIM_FILES}")
list(APPEND TESTBENCH_FILES "${simple_axi_uvm_SIM_FILES}")
list(APPEND TESTBENCH_FILES "${simple_ics_uvm_SIM_FILES}")
#list(APPEND TESTBENCH_FILES "${UVM_AXI_FILES}")

#ip_repo_path list
set(IP_REPO_PATH "")
list(APPEND IP_REPO_PATH "${CMAKE_CURRENT_BINARY_DIR}")

#vivado create_project name(currently fixed to project_1)
set(VIVADO_PRJ_NAME "project_1")
set(SIMSET_NAME "sim_1")

#create block design scripts
set(BLOCK_DESIGN_TCL "${CMAKE_CURRENT_SOURCE_DIR}/scripts/blockdesign.tcl")

#rtl package flag (0:block_design, 1:rtl module)
set(RTL_PACKAGE_FLAG "0")

project_generation( ${PROJECT_NAME} ${VENDOR} ${LIBRARY_NAME} ${TARGET_DEVICE} "${SRC_FILES}" "${TESTBENCH_FILES}" "${IP_REPO_PATH}")
project_add_bd( ${PROJECT_NAME} ${BLOCK_DESIGN_TCL} prj_gen_${PROJECT_NAME})
export_ip( ${VENDOR} ${LIBRARY_NAME} ${RTL_PACKAGE_FLAG})

add_dependencies( prj_gen_${PROJECT_NAME} ics_if_main)
add_dependencies( prj_gen_${PROJECT_NAME} ics_if_tx)
add_dependencies( prj_gen_${PROJECT_NAME} ics_if_rx)
add_dependencies( ${PROJECT_NAME} add_bd_${PROJECT_NAME} )

#simulation
set(DEPENDENCIES ${PROJECT_NAME})
set(SIMULATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/sim")
file(MAKE_DIRECTORY ${SIMULATION_DIR}/work/xsim)
add_custom_target(gen_all DEPENDS ${DEPENDENCIES})
add_custom_target(compile_all DEPENDS gen_all)
add_custom_target(elaborate_all DEPENDS compile_all)
add_custom_target(sim_all DEPENDS elaborate_all)

set(INCLUDE_DIRS_VER "")
#list(APPEND INCLUDE_DIRS_VER "-i ${CMAKE_CURRENT_SOURCE_DIR}/test/lib/uvm_gp_scoreboard")
list(APPEND INCLUDE_DIRS_VER "-i ${CMAKE_CURRENT_SOURCE_DIR}/test/include")
list(APPEND INCLUDE_DIRS_VER "${simple_axi_uvm_DEFINITIONS_VLOG} ${simple_ics_uvm_DEFINITIONS_VLOG} ${gp_scoreboard_DEFINITIONS_VLOG}")

#simulation dependencies
file(GLOB TB_ICS_IF_DEPENDENCIES test/include/*.svh test/src/tb_ics_if_uvm*.sv)

set(ADDITIONAL_VLOG_OPTS "-L uvm -d VIVADO_SIM ${INCLUDE_DIRS_VER}")
set(ADDITIONAL_ELAB_OPTS "-L uvm")
set(ADDITIONAL_XSIM_OPTS "-testplusarg UVM_TESTNAME")

add_sim(tb_interval_timer ${SIMULATION_DIR} "${DEPENDENCIES};${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_interval_timer.v" "${ADDITIONAL_VLOG_OPTS}" "${ADDITIONAL_ELAB_OPTS}" "${ADDITIONAL_XSIM_OPTS}" "")
add_sim(tb_uvm_test ${SIMULATION_DIR} "${DEPENDENCIES};${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_uvm_test.sv" "${ADDITIONAL_VLOG_OPTS}" "${ADDITIONAL_ELAB_OPTS}" "${ADDITIONAL_XSIM_OPTS}" tb_interval_timer)
add_sim(tb_ics_if_uvm ${SIMULATION_DIR} "${DEPENDENCIES};${TB_ICS_IF_DEPENDENCIES}" "${ADDITIONAL_VLOG_OPTS}" "${ADDITIONAL_ELAB_OPTS}" "${ADDITIONAL_XSIM_OPTS}" tb_uvm_test)

#document generation
add_pandoc(ics_if "${CMAKE_CURRENT_SOURCE_DIR}/docs/ics_if.md")

if(PRINT_VARIABLES)
    message(STATUS "*** dump start cmake variables ***")
    get_cmake_property(_variableNames VARIABLES)
    foreach(_variableName ${_variableNames})
            message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "*** dump end ***")
endif()
