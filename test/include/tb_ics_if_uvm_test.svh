// tb_ics_if_uvm_test.svh
//      This file implements the test for tb_ics_if_uvm.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

class ics_if_single_communication_test extends uvm_test;
    `uvm_component_utils(ics_if_single_communication_test)
    `uvm_new_func
    tb_ics_if_uvm_env env;
    virtual function void build_phase(uvm_phase phase);
        env=tb_ics_if_uvm_env::type_id::create("tb_ics_if_uvm_env",this);
        super.build_phase(phase);
    endfunction

    task run_phase(uvm_phase phase);
        //declaration
        simple_axi_seq_item bram_exp_item;
        init_all_register_seq               seq  = init_all_register_seq::type_id::create("seq2");
        all_initialized_register_read_seq   seq2 = all_initialized_register_read_seq::type_id::create("seq2");
        init_bram_seq                       seq3 = init_bram_seq::type_id::create("seq3");
        read_cyclic0_bram_seq               seq4 = read_cyclic0_bram_seq::type_id::create("seq4");
        //expectation
        //init
        //cyclic0
        bram_exp_item = new(.addr(32'h0000_0000),
                            .data({ 32'h0201_0080,32'h0302_0081,32'h0403_0082,32'h0504_0083,32'h0605_0084,32'h0706_0085,32'h0807_0086,32'h0908_0087,
                                    32'h0a09_0088,32'h0b0a_0089,32'h0c0b_008a,32'h0d0c_008b,32'h0e0d_008c,32'h0f0e_008d,32'h100f_008e,32'h1110_008f}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h0000_0040),
                            .data({ 32'h1211_0090,32'h1312_0091,32'h1413_0092,32'h1514_0093,32'h1615_0094,32'h1716_0095,32'h1817_0096,32'h1918_0097,
                                    32'h1a19_0098,32'h1b1a_0099,32'h1c1b_009a,32'h1d1c_009b,32'h1e1d_009c,32'h1f1e_009d,32'h201f_009e,32'h2120_009f}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //cyclic1
        bram_exp_item = new(.addr(32'h0000_0100),
                            .data({ 32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                                    32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h0000_0140),
                            .data({ 32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                                    32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //after cyclic0 communication expectation
        bram_exp_item = new(.addr(32'h00000400),
                            .data({ 32'h02010100,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,
                                    32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h00000440),
                            .data({ 32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,
                                    32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //after second cyclic0 communication expectation
        bram_exp_item = new(.addr(32'h00000400),
                            .data({ 32'h02010200,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,
                                    32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h00000440),
                            .data({ 32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,
                                    32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000,32'h00000000}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        uvm_report_info("TEST", "simple_axi_uvm_test_example running");
        //sequence
        phase.raise_objection(this);
        seq.start(env.agent.sequencer);
        seq2.start(env.agent.sequencer);
        seq3.start(env.agent_mem.sequencer);
        #2000000;
        seq4.start(env.agent_mem.sequencer);
        env.agent_mem.driver.wait_trans_done();
        #1300000;
        seq4.start(env.agent_mem.sequencer);
        env.agent_mem.driver.wait_trans_done();
        #5000000;
        phase.drop_objection(this);
    endtask
endclass


class ics_if_multi_communication_test extends uvm_test;
    `uvm_component_utils(ics_if_multi_communication_test)
    `uvm_new_func
    tb_ics_if_uvm_env env;
    virtual function void build_phase(uvm_phase phase);
        env=tb_ics_if_uvm_env::type_id::create("tb_ics_if_uvm_env",this);
        super.build_phase(phase);
    endfunction

    task run_phase(uvm_phase phase);
        //declaration
        simple_axi_seq_item bram_exp_item;
        init_all_register_seq               seq  = init_all_register_seq::type_id::create("seq2");
        all_initialized_register_read_seq   seq2 = all_initialized_register_read_seq::type_id::create("seq2");
        multi_servo_config_seq              seq5 = multi_servo_config_seq::type_id::create("seq5");
        init_bram_seq                       seq3 = init_bram_seq::type_id::create("seq3");
        read_cyclic0_bram_seq               seq4 = read_cyclic0_bram_seq::type_id::create("seq4");
        //expectation
        //init
        //cyclic0
        bram_exp_item = new(.addr(32'h0000_0000),
                            .data({ 32'h0201_0080,32'h0302_0081,32'h0403_0082,32'h0504_0083,32'h0605_0084,32'h0706_0085,32'h0807_0086,32'h0908_0087,
                                    32'h0a09_0088,32'h0b0a_0089,32'h0c0b_008a,32'h0d0c_008b,32'h0e0d_008c,32'h0f0e_008d,32'h100f_008e,32'h1110_008f}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h0000_0040),
                            .data({ 32'h1211_0090,32'h1312_0091,32'h1413_0092,32'h1514_0093,32'h1615_0094,32'h1716_0095,32'h1817_0096,32'h1918_0097,
                                    32'h1a19_0098,32'h1b1a_0099,32'h1c1b_009a,32'h1d1c_009b,32'h1e1d_009c,32'h1f1e_009d,32'h201f_009e,32'h2120_009f}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //cyclic1
        bram_exp_item = new(.addr(32'h0000_0100),
                            .data({ 32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                                    32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h0000_0140),
                            .data({ 32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                                    32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                            .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //after cyclic0 communication expectation
        bram_exp_item = new(.addr(32'h00000400),
                            .data({ 32'h02010100,32'h03020101,32'h04030102,32'h05040103,32'h06050104,32'h07060105,32'h08070106,32'h09080107,
                                    32'h0a090108,32'h0b0a0109,32'h0c0b010a,32'h0d0c010b,32'h0e0d010c,32'h0f0e010d,32'h100f010e,32'h1110010f}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h00000440),
                            .data({ 32'h12110110,32'h13120111,32'h14130112,32'h15140113,32'h16150114,32'h17160115,32'h18170116,32'h19180117,
                                    32'h1a190118,32'h1b1a0119,32'h1c1b011a,32'h1d1c011b,32'h1e1d011c,32'h1f1e011d,32'h201f011e,32'h2120011f}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        //after second cyclic0 communication expectation
        bram_exp_item = new(.addr(32'h00000400),
                            .data({ 32'h02010200,32'h03020201,32'h04030202,32'h05040203,32'h06050204,32'h07060205,32'h08070206,32'h09080207,
                                    32'h0a090208,32'h0b0a0209,32'h0c0b020a,32'h0d0c020b,32'h0e0d020c,32'h0f0e020d,32'h100f020e,32'h1110020f}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        bram_exp_item = new(.addr(32'h00000440),
                            .data({ 32'h12110210,32'h13120211,32'h14130212,32'h15140213,32'h16150214,32'h17160215,32'h18170216,32'h19180217,
                                    32'h1a190218,32'h1b1a0219,32'h1c1b021a,32'h1d1c021b,32'h1e1d021c,32'h1f1e021d,32'h201f021e,32'h2120021f}),
                            .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        env.axi4_bram_scrbd.write_exp(bram_exp_item);
        uvm_report_info("TEST", "simple_axi_uvm_test_example running");
        //sequence
        phase.raise_objection(this);
        seq.start(env.agent.sequencer);
        seq2.start(env.agent.sequencer);
        seq5.start(env.agent.sequencer);
        seq3.start(env.agent_mem.sequencer);
        #42000000;
        seq4.start(env.agent_mem.sequencer);
        env.agent_mem.driver.wait_trans_done();
        #21000000;
        seq4.start(env.agent_mem.sequencer);
        env.agent_mem.driver.wait_trans_done();
        #5000000;
        phase.drop_objection(this);
    endtask
endclass
