// tb_ics_if_uvm_env.svh
//      This file implements the testbench environment for tb_ics_if_uvm_pkg.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

class tb_ics_if_uvm_env extends uvm_env;
    `uvm_component_utils(tb_ics_if_uvm_env)
    `uvm_new_func
    simple_axi_master_agent agent;
    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        agent = simple_axi_master_agent::type_id::create("simple_axi_master_agent",this);
    endfunction
endclass

