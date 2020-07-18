cmake_minimum_required(VERSION 3.1)
find_program(VIVADO_COMMAND
		NAME vivado
		HINTS ENV var)

find_file(HELPER_SCRIPT_IPX
		NAME create_ip.tcl
		HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(HELPER_SCRIPT_PRJ_GEN
		NAME project_generation.tcl
		HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(HELPER_SCRIPT_IMPORT_BD
		NAME vivado_import_bd.tcl
		HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(HELPER_SCRIPT_EXPORT_SIM
		NAME vivado_export_sim.tcl
		HINTS $ENV{VIVADO_CMAKE_HELPER})

find_file(VIVADO_IPX_EXPORT_CMAKE
		NAME vivado_ipx_export.cmake
		HINTS $ENV{VIVADO_CMAKE_HELPER}
		REQUIRED)

#version fixed to 1.0 due to vivado 2019.2 restriction.
project(ics_if
	VERSION 1.0
	LANGUAGES NONE)

set(CP "cp")
set(CP_OPTION "-rf")
set(MV "mv")

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
list(APPEND TESTBENCH_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/src/tb_ics_if.sv")
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
set(SIM_BLOCK_DESIGN_TCL "${CMAKE_CURRENT_SOURCE_DIR}/scripts/sim_ics_if.tcl")
add_custom_command(
	OUTPUT import_bd_sim_ics_if.timestamp
	COMMAND ${VIVADO_COMMAND} -mode batch -source ${HELPER_SCRIPT_IMPORT_BD} -tclargs ${SIM_BLOCK_DESIGN_TCL}
	COMMAND ${CMAKE_COMMAND} -E touch import_bd_sim_ics_if.timestamp
	)
add_custom_target( import_bd_sim_ics_if
	DEPENDS ${PROJECT_NAME} import_bd_sim_ics_if.timestamp
	)

set(DEPENDENCIES "import_bd_sim_ics_if")
set(SIMULATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/sim")
set(SIM_TARGET "tb_ics_if")
add_custom_command(
	OUTPUT ${SIMULATION_DIR}/work/xsim/compile_${SIM_TARGET}.sh
	COMMAND ${VIVADO_COMMAND} -mode batch -source ${HELPER_SCRIPT_EXPORT_SIM} -tclargs
		${SIM_TARGET}
		${SIMULATION_DIR}
		COMMAND ${CP} ${CP_OPTION} ${VIVADO_PRJ_NAME}/${VIVADO_PRJ_NAME}.sim/${SIMSET_NAME}/behav/xsim ${SIMULATION_DIR}/work
	COMMAND ${MV} ${SIMULATION_DIR}/work/xsim/compile.sh ${SIMULATION_DIR}/work/xsim/compile_${SIM_TARGET}.sh
	COMMAND ${MV} ${SIMULATION_DIR}/work/xsim/elaborate.sh ${SIMULATION_DIR}/work/xsim/elaborate_${SIM_TARGET}.sh
	COMMAND ${MV} ${SIMULATION_DIR}/work/xsim/simulate.sh ${SIMULATION_DIR}/work/xsim/simulate_${SIM_TARGET}.sh
	)

add_custom_command(
	OUTPUT ${SIMULATION_DIR}/work/xsim/${SIM_TARGET}.wdb
	COMMAND ./compile_${SIM_TARGET}.sh
	COMMAND ./elaborate_${SIM_TARGET}.sh
	COMMAND ./simulate_${SIM_TARGET}.sh
	WORKING_DIRECTORY ${SIMULATION_DIR}/work/xsim
	DEPENDS ${SIMULATION_DIR}/work/xsim/compile_${SIM_TARGET}.sh
)	
add_custom_target( sim_${SIM_TARGET}
	DEPENDS ${PROJECT_NAME} ${SIMULATION_DIR}/work/xsim/${SIM_TARGET}.wdb ${DEPENDENCIES}
	)

if(PRINT_VARIABLES)
	message(STATUS "*** dump start cmake variables ***")
	get_cmake_property(_variableNames VARIABLES)
	foreach(_variableName ${_variableNames})
	        message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
	message(STATUS "*** dump end ***")
endif()
