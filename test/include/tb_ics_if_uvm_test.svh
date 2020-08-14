// tb_ics_if_uvm_test.svh
//      This file implements the test for tb_ics_if_uvm.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

class ics_if_basic_tx_test extends uvm_test;
    `uvm_component_utils(ics_if_basic_tx_test)
    `uvm_new_func
    tb_ics_if_uvm_env env;
    virtual function void build_phase(uvm_phase phase);
        env=tb_ics_if_uvm_env::type_id::create("tb_ics_if_uvm_env",this);
        super.build_phase(phase);
    endfunction

    task run_phase(uvm_phase phase);
        init_all_register_seq               seq  = init_all_register_seq::type_id::create("seq2");
        all_initialized_register_read_seq   seq2 = all_initialized_register_read_seq::type_id::create("seq2");
        init_bram_seq                       seq3 = init_bram_seq::type_id::create("seq3");
        uvm_report_info("TEST", "simple_axi_uvm_test_example running");
        phase.raise_objection(this);
        seq.start(env.agent.sequencer);
        seq2.start(env.agent.sequencer);
        seq3.start(env.agent_mem.sequencer);
        env.agent_mem.driver.wait_trans_done();
        #20000000;
        phase.drop_objection(this);
    endtask
endclass
