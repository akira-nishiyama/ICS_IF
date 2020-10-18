
################################################################
# This is a generated script based on design: ics_if
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2019.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_msg_id "BD_TCL-109" "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source ics_if_script.tcl


# The design that will be created by this Tcl script contains the following 
# module references:
# dff_with_we, dff_with_we, interval_timer

# Please add the sources of those modules before sourcing this Tcl script.

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xczu3eg-sbva484-1-e
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name ics_if

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_msg_id "BD_TCL-001" "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_msg_id "BD_TCL-002" "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_msg_id "BD_TCL-003" "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_msg_id "BD_TCL-004" "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_msg_id "BD_TCL-005" "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_msg_id "BD_TCL-114" "ERROR" $errMsg}
   return $nRet
}

set bCheckIPsPassed 1
##################################################################
# CHECK IPs
##################################################################
set bCheckIPs 1
if { $bCheckIPs == 1 } {
   set list_check_ips "\ 
xilinx.com:ip:axi_bram_ctrl:4.1\
xilinx.com:ip:axis_data_fifo:2.0\
xilinx.com:ip:blk_mem_gen:8.4\
xilinx.com:ip:fifo_generator:13.2\
Akira_Nishiyama:hls:ics_if_main:0.1\
Akira_Nishiyama:hls:ics_if_rx:0.1\
Akira_Nishiyama:hls:ics_if_tx:0.1\
xilinx.com:ip:util_ds_buf:2.1\
xilinx.com:ip:util_vector_logic:2.0\
xilinx.com:ip:xlconstant:1.1\
"

   set list_ips_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

}

##################################################################
# CHECK Modules
##################################################################
set bCheckModules 1
if { $bCheckModules == 1 } {
   set list_check_mods "\ 
dff_with_we\
dff_with_we\
interval_timer\
"

   set list_mods_missing ""
   common::send_msg_id "BD_TCL-006" "INFO" "Checking if the following modules exist in the project's sources: $list_check_mods ."

   foreach mod_vlnv $list_check_mods {
      if { [can_resolve_reference $mod_vlnv] == 0 } {
         lappend list_mods_missing $mod_vlnv
      }
   }

   if { $list_mods_missing ne "" } {
      catch {common::send_msg_id "BD_TCL-115" "ERROR" "The following module(s) are not found in the project: $list_mods_missing" }
      common::send_msg_id "BD_TCL-008" "INFO" "Please add source files for the missing module(s) above."
      set bCheckIPsPassed 0
   }
}

if { $bCheckIPsPassed != 1 } {
  common::send_msg_id "BD_TCL-1003" "WARNING" "Will not continue with creation of design due to the error(s) above."
  return 3
}

##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_msg_id "BD_TCL-100" "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_msg_id "BD_TCL-101" "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set S_AXI_for_bram [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 S_AXI_for_bram ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {15} \
   CONFIG.ARUSER_WIDTH {0} \
   CONFIG.AWUSER_WIDTH {0} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {32} \
   CONFIG.HAS_BRESP {1} \
   CONFIG.HAS_BURST {1} \
   CONFIG.HAS_CACHE {1} \
   CONFIG.HAS_LOCK {1} \
   CONFIG.HAS_PROT {1} \
   CONFIG.HAS_QOS {0} \
   CONFIG.HAS_REGION {0} \
   CONFIG.HAS_RRESP {1} \
   CONFIG.HAS_WSTRB {1} \
   CONFIG.ID_WIDTH {0} \
   CONFIG.MAX_BURST_LENGTH {256} \
   CONFIG.NUM_READ_OUTSTANDING {2} \
   CONFIG.NUM_READ_THREADS {1} \
   CONFIG.NUM_WRITE_OUTSTANDING {2} \
   CONFIG.NUM_WRITE_THREADS {1} \
   CONFIG.PROTOCOL {AXI4} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {0} \
   CONFIG.SUPPORTS_NARROW_BURST {1} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {0} \
   ] $S_AXI_for_bram

  set s_axi_for_reg [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:aximm_rtl:1.0 s_axi_for_reg ]
  set_property -dict [ list \
   CONFIG.ADDR_WIDTH {16} \
   CONFIG.ARUSER_WIDTH {0} \
   CONFIG.AWUSER_WIDTH {0} \
   CONFIG.BUSER_WIDTH {0} \
   CONFIG.DATA_WIDTH {32} \
   CONFIG.HAS_BRESP {1} \
   CONFIG.HAS_BURST {0} \
   CONFIG.HAS_CACHE {0} \
   CONFIG.HAS_LOCK {0} \
   CONFIG.HAS_PROT {0} \
   CONFIG.HAS_QOS {0} \
   CONFIG.HAS_REGION {0} \
   CONFIG.HAS_RRESP {1} \
   CONFIG.HAS_WSTRB {1} \
   CONFIG.ID_WIDTH {0} \
   CONFIG.MAX_BURST_LENGTH {1} \
   CONFIG.NUM_READ_OUTSTANDING {1} \
   CONFIG.NUM_READ_THREADS {1} \
   CONFIG.NUM_WRITE_OUTSTANDING {1} \
   CONFIG.NUM_WRITE_THREADS {1} \
   CONFIG.PROTOCOL {AXI4LITE} \
   CONFIG.READ_WRITE_MODE {READ_WRITE} \
   CONFIG.RUSER_BITS_PER_BYTE {0} \
   CONFIG.RUSER_WIDTH {0} \
   CONFIG.SUPPORTS_NARROW_BURST {0} \
   CONFIG.WUSER_BITS_PER_BYTE {0} \
   CONFIG.WUSER_WIDTH {0} \
   ] $s_axi_for_reg


  # Create ports
  set ap_clk_0 [ create_bd_port -dir I -type clk ap_clk_0 ]
  set_property -dict [ list \
   CONFIG.ASSOCIATED_BUSIF {S_AXI_for_bram:s_axi_for_reg} \
 ] $ap_clk_0
  set ap_rst_n_0 [ create_bd_port -dir I -type rst ap_rst_n_0 ]
  set ics_sig [ create_bd_port -dir IO -from 0 -to 0 ics_sig ]

  # Create instance: axi_bram_ctrl_0, and set properties
  set axi_bram_ctrl_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_bram_ctrl:4.1 axi_bram_ctrl_0 ]
  set_property -dict [ list \
   CONFIG.SINGLE_PORT_BRAM {1} \
 ] $axi_bram_ctrl_0

  # Create instance: axis_data_fifo_0, and set properties
  set axis_data_fifo_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axis_data_fifo:2.0 axis_data_fifo_0 ]
  set_property -dict [ list \
   CONFIG.FIFO_DEPTH {64} \
 ] $axis_data_fifo_0

  # Create instance: blk_mem_gen_0, and set properties
  set blk_mem_gen_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 blk_mem_gen_0 ]
  set_property -dict [ list \
   CONFIG.Byte_Size {8} \
   CONFIG.EN_SAFETY_CKT {true} \
   CONFIG.Enable_32bit_Address {true} \
   CONFIG.Enable_B {Use_ENB_Pin} \
   CONFIG.Memory_Type {True_Dual_Port_RAM} \
   CONFIG.Port_B_Clock {100} \
   CONFIG.Port_B_Enable_Rate {100} \
   CONFIG.Port_B_Write_Rate {50} \
   CONFIG.Register_PortA_Output_of_Memory_Primitives {false} \
   CONFIG.Register_PortB_Output_of_Memory_Primitives {false} \
   CONFIG.Use_Byte_Write_Enable {true} \
   CONFIG.Use_RSTA_Pin {true} \
   CONFIG.Use_RSTB_Pin {true} \
   CONFIG.use_bram_block {BRAM_Controller} \
 ] $blk_mem_gen_0

  # Create instance: dff_with_we_0, and set properties
  set block_name dff_with_we
  set block_cell_name dff_with_we_0
  if { [catch {set dff_with_we_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $dff_with_we_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: dff_with_we_1, and set properties
  set block_name dff_with_we
  set block_cell_name dff_with_we_1
  if { [catch {set dff_with_we_1 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $dff_with_we_1 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
    set_property -dict [ list \
   CONFIG.init_val {"1"} \
 ] $dff_with_we_1

  # Create instance: fifo_generator_0, and set properties
  set fifo_generator_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:13.2 fifo_generator_0 ]
  set_property -dict [ list \
   CONFIG.Data_Count_Width {5} \
   CONFIG.Empty_Threshold_Assert_Value {4} \
   CONFIG.Empty_Threshold_Negate_Value {5} \
   CONFIG.Fifo_Implementation {Common_Clock_Distributed_RAM} \
   CONFIG.Full_Threshold_Assert_Value {15} \
   CONFIG.Full_Threshold_Negate_Value {14} \
   CONFIG.Input_Data_Width {1} \
   CONFIG.Input_Depth {16} \
   CONFIG.Output_Data_Width {1} \
   CONFIG.Output_Depth {16} \
   CONFIG.Performance_Options {First_Word_Fall_Through} \
   CONFIG.Read_Data_Count_Width {5} \
   CONFIG.Use_Embedded_Registers {false} \
   CONFIG.Use_Extra_Logic {true} \
   CONFIG.Write_Data_Count_Width {5} \
 ] $fifo_generator_0

  # Create instance: ics_if_main_0, and set properties
  set ics_if_main_0 [ create_bd_cell -type ip -vlnv Akira_Nishiyama:hls:ics_if_main:0.1 ics_if_main_0 ]

  # Create instance: ics_if_rx_0, and set properties
  set ics_if_rx_0 [ create_bd_cell -type ip -vlnv Akira_Nishiyama:hls:ics_if_rx:0.1 ics_if_rx_0 ]

  # Create instance: ics_if_tx_0, and set properties
  set ics_if_tx_0 [ create_bd_cell -type ip -vlnv Akira_Nishiyama:hls:ics_if_tx:0.1 ics_if_tx_0 ]

  # Create instance: interval_timer_0, and set properties
  set block_name interval_timer
  set block_cell_name interval_timer_0
  if { [catch {set interval_timer_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_msg_id "BD_TCL-105" "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $interval_timer_0 eq "" } {
     catch {common::send_msg_id "BD_TCL-106" "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: util_ds_buf_0, and set properties
  set util_ds_buf_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.1 util_ds_buf_0 ]
  set_property -dict [ list \
   CONFIG.C_BUF_TYPE {IOBUF} \
 ] $util_ds_buf_0

  # Create instance: util_vector_logic_0, and set properties
  set util_vector_logic_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_0 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_0

  # Create instance: util_vector_logic_1, and set properties
  set util_vector_logic_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_1 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {and} \
   CONFIG.C_SIZE {1} \
 ] $util_vector_logic_1

  # Create instance: util_vector_logic_2, and set properties
  set util_vector_logic_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_2 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_2

  # Create instance: util_vector_logic_3, and set properties
  set util_vector_logic_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_3 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_3

  # Create instance: util_vector_logic_4, and set properties
  set util_vector_logic_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_vector_logic:2.0 util_vector_logic_4 ]
  set_property -dict [ list \
   CONFIG.C_OPERATION {not} \
   CONFIG.C_SIZE {1} \
   CONFIG.LOGO_FILE {data/sym_notgate.png} \
 ] $util_vector_logic_4

  # Create instance: xlconstant_0, and set properties
  set xlconstant_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 xlconstant_0 ]

  # Create interface connections
  connect_bd_intf_net -intf_net S_AXI_0_1 [get_bd_intf_ports S_AXI_for_bram] [get_bd_intf_pins axi_bram_ctrl_0/S_AXI]
  connect_bd_intf_net -intf_net axi_bram_ctrl_0_BRAM_PORTA [get_bd_intf_pins axi_bram_ctrl_0/BRAM_PORTA] [get_bd_intf_pins blk_mem_gen_0/BRAM_PORTB]
  connect_bd_intf_net -intf_net axis_data_fifo_0_M_AXIS [get_bd_intf_pins axis_data_fifo_0/M_AXIS] [get_bd_intf_pins ics_if_main_0/ics_rx_char_i_V_V]
  connect_bd_intf_net -intf_net ics_if_main_0_communication_memory_V_PORTA [get_bd_intf_pins blk_mem_gen_0/BRAM_PORTA] [get_bd_intf_pins ics_if_main_0/communication_memory_V_PORTA]
  connect_bd_intf_net -intf_net ics_if_main_0_ics_tx_char_o_V_V [get_bd_intf_pins ics_if_main_0/ics_tx_char_o_V] [get_bd_intf_pins ics_if_tx_0/ics_char_i_V]
  connect_bd_intf_net -intf_net ics_if_rx_0_ics_char_o_V_V [get_bd_intf_pins axis_data_fifo_0/S_AXIS] [get_bd_intf_pins ics_if_rx_0/ics_char_o_V_V]
  connect_bd_intf_net -intf_net s_axi_slv0_0_1 [get_bd_intf_ports s_axi_for_reg] [get_bd_intf_pins ics_if_main_0/s_axi_slv0]

  # Create port connections
  connect_bd_net -net Net [get_bd_ports ics_sig] [get_bd_pins util_ds_buf_0/IOBUF_IO_IO]
  connect_bd_net -net ap_clk_0_1 [get_bd_ports ap_clk_0] [get_bd_pins axi_bram_ctrl_0/s_axi_aclk] [get_bd_pins axis_data_fifo_0/s_axis_aclk] [get_bd_pins dff_with_we_0/clk] [get_bd_pins dff_with_we_1/clk] [get_bd_pins fifo_generator_0/clk] [get_bd_pins ics_if_main_0/ap_clk] [get_bd_pins ics_if_rx_0/ap_clk] [get_bd_pins ics_if_tx_0/ap_clk] [get_bd_pins interval_timer_0/ap_clk]
  connect_bd_net -net ap_rst_n_0_1 [get_bd_ports ap_rst_n_0] [get_bd_pins axi_bram_ctrl_0/s_axi_aresetn] [get_bd_pins axis_data_fifo_0/s_axis_aresetn] [get_bd_pins ics_if_main_0/ap_rst_n] [get_bd_pins ics_if_tx_0/ap_rst_n] [get_bd_pins interval_timer_0/ap_rst_n] [get_bd_pins util_vector_logic_1/Op2] [get_bd_pins util_vector_logic_2/Op1]
  connect_bd_net -net dff_with_we_0_q [get_bd_pins dff_with_we_0/q] [get_bd_pins util_ds_buf_0/IOBUF_IO_I]
  connect_bd_net -net dff_with_we_1_q [get_bd_pins dff_with_we_1/q] [get_bd_pins util_ds_buf_0/IOBUF_IO_T]
  connect_bd_net -net fifo_generator_0_dout [get_bd_pins fifo_generator_0/dout] [get_bd_pins ics_if_main_0/cyclic0_start_i_V_V_dout]
  connect_bd_net -net fifo_generator_0_empty [get_bd_pins fifo_generator_0/empty] [get_bd_pins util_vector_logic_3/Op1]
  connect_bd_net -net ics_if_main_0_bit_period_o_V [get_bd_pins ics_if_main_0/bit_period_o_V] [get_bd_pins ics_if_rx_0/bit_period_i_V] [get_bd_pins ics_if_tx_0/bit_period_i_V]
  connect_bd_net -net ics_if_main_0_cyclic0_enable_o_V [get_bd_pins ics_if_main_0/cyclic0_enable_o_V] [get_bd_pins interval_timer_0/interrupt_en]
  connect_bd_net -net ics_if_main_0_cyclic0_interval_o_V [get_bd_pins ics_if_main_0/cyclic0_interval_o_V] [get_bd_pins interval_timer_0/interval_i]
  connect_bd_net -net ics_if_main_0_cyclic0_start_i_V_V_read [get_bd_pins fifo_generator_0/rd_en] [get_bd_pins ics_if_main_0/cyclic0_start_i_V_V_read]
  connect_bd_net -net ics_if_main_0_ics_rx_char_rst_o_V [get_bd_pins ics_if_main_0/ics_rx_char_rst_o_V] [get_bd_pins util_vector_logic_0/Op1]
  connect_bd_net -net ics_if_tx_0_ics_sig_dir_o_V_V_din [get_bd_pins ics_if_tx_0/ics_sig_dir_o_V_V_din] [get_bd_pins util_vector_logic_4/Op1]
  connect_bd_net -net ics_if_tx_0_ics_sig_dir_o_V_V_write [get_bd_pins dff_with_we_1/we] [get_bd_pins ics_if_tx_0/ics_sig_dir_o_V_V_write]
  connect_bd_net -net ics_if_tx_0_ics_sig_o_V_V_din [get_bd_pins dff_with_we_0/d] [get_bd_pins ics_if_tx_0/ics_sig_o_V_V_din]
  connect_bd_net -net ics_if_tx_0_ics_sig_o_V_V_write [get_bd_pins dff_with_we_0/we] [get_bd_pins ics_if_tx_0/ics_sig_o_V_V_write]
  connect_bd_net -net loadable_counter_0_interrupt_o [get_bd_pins fifo_generator_0/din] [get_bd_pins fifo_generator_0/wr_en] [get_bd_pins interval_timer_0/interrupt_o]
  connect_bd_net -net util_ds_buf_0_IOBUF_IO_O [get_bd_pins ics_if_rx_0/ics_sig_i_V_V] [get_bd_pins util_ds_buf_0/IOBUF_IO_O]
  connect_bd_net -net util_vector_logic_0_Res [get_bd_pins util_vector_logic_0/Res] [get_bd_pins util_vector_logic_1/Op1]
  connect_bd_net -net util_vector_logic_1_Res [get_bd_pins ics_if_rx_0/ap_rst_n] [get_bd_pins util_vector_logic_1/Res]
  connect_bd_net -net util_vector_logic_2_Res [get_bd_pins dff_with_we_0/rst] [get_bd_pins dff_with_we_1/rst] [get_bd_pins fifo_generator_0/srst] [get_bd_pins util_vector_logic_2/Res]
  connect_bd_net -net util_vector_logic_3_Res [get_bd_pins ics_if_main_0/cyclic0_start_i_V_V_empty_n] [get_bd_pins util_vector_logic_3/Res]
  connect_bd_net -net util_vector_logic_4_Res [get_bd_pins dff_with_we_1/d] [get_bd_pins util_vector_logic_4/Res]
  connect_bd_net -net xlconstant_0_dout [get_bd_pins ics_if_rx_0/ap_start] [get_bd_pins ics_if_rx_0/ics_sig_i_V_V_ap_vld] [get_bd_pins ics_if_tx_0/ap_start] [get_bd_pins ics_if_tx_0/ics_sig_dir_o_V_V_full_n] [get_bd_pins ics_if_tx_0/ics_sig_o_V_V_full_n] [get_bd_pins xlconstant_0/dout]

  # Create address segments
  assign_bd_address -offset 0x00001000 -range 0x00001000 -target_address_space [get_bd_addr_spaces S_AXI_for_bram] [get_bd_addr_segs axi_bram_ctrl_0/S_AXI/Mem0] -force
  assign_bd_address -offset 0x00000000 -range 0x00000800 -target_address_space [get_bd_addr_spaces s_axi_for_reg] [get_bd_addr_segs ics_if_main_0/s_axi_slv0/Reg] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


