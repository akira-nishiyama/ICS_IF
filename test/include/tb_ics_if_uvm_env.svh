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
    simple_axi_master_agent agent_mem;
    simple_ics_slave_env ics_env;
    gp_scoreboard#(simple_axi_seq_item) axi4_lite_scrbd;
    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        agent     = simple_axi_master_agent::type_id::create("agent",this);
        agent_mem = simple_axi_master_agent::type_id::create("agent_mem",this);
        ics_env   = simple_ics_slave_env::type_id::create("ics_env",this);
        axi4_lite_scrbd = gp_scoreboard#(simple_axi_seq_item)::type_id::create("axi4_lite_scrbd",this);
    endfunction

    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        agent.driver.item_port.connect(axi4_lite_scrbd.ap_exp);
        agent.monitor.item_port.connect(axi4_lite_scrbd.ap_obs);
    endfunction
endclass

