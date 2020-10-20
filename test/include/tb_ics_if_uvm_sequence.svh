// tb_ics_if_uvm_sequence.svh
//      This file implements the sequence for tb_ics_if_uvm_pkg.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

//axi4-lite sequence

class all_initialized_register_read_seq extends simple_axi_master_base_sequence;
    `uvm_object_utils(all_initialized_register_read_seq)
    function new(string name="all_register_read_seq");
        super.new(name);
    endfunction
    virtual task body();
        simple_axi_seq_item trans_item;
        `uvm_create(trans_item);
        transfer_item(.trans_item(trans_item), .addr(32'h00000000), .data({32'h0000_0081}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000004), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000008), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h0000000c), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000010), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000018), .data({32'h0000_0001}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000020), .data({32'h8000_0002}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000028), .data({32'h8000_0002}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000030), .data({32'h8000_03e8}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000038), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
    endtask
endclass

class init_all_register_seq extends simple_axi_master_base_sequence;
    `uvm_object_utils(init_all_register_seq)
    function new(string name="init_all_register_seq");
        super.new(name);
    endfunction

    virtual task body();
        simple_axi_seq_item trans_item;
        `uvm_create(trans_item);
        transfer_item(.trans_item(trans_item), .addr(32'h00000010), .data({32'h0000_0000}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000018), .data({32'h0000_0001}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000020), .data({32'h8000_0002}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000028), .data({32'h8000_0002}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000030), .data({32'h8000_03e8}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000000), .data({32'h0000_0081}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
    endtask

endclass

class multi_servo_config_seq extends simple_axi_master_base_sequence;
    `uvm_object_utils(multi_servo_config_seq)
    function new(string name="init_all_register_seq");
        super.new(name);
    endfunction

    virtual task body();
        simple_axi_seq_item trans_item;
        `uvm_create(trans_item);
        transfer_item(.trans_item(trans_item), .addr(32'h00000018), .data({32'h0000_0020}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(.trans_item(trans_item), .addr(32'h00000020), .data({32'h8000_0021}), .length(0), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
    endtask

endclass

//axi-bram sequence
class init_bram_seq extends simple_axi_master_base_sequence;
    `uvm_object_utils(init_bram_seq)
    function new(string name="init_bram_seq");
        super.new(name);
    endfunction

    virtual task body();
        simple_axi_seq_item trans_item;
        `uvm_create(trans_item);
        //cyclic0
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0000),
                        .data({32'h0201_0080,32'h0302_0081,32'h0403_0082,32'h0504_0083,32'h0605_0084,32'h0706_0085,32'h0807_0086,32'h0908_0087,
                               32'h0a09_0088,32'h0b0a_0089,32'h0c0b_008a,32'h0d0c_008b,32'h0e0d_008c,32'h0f0e_008d,32'h100f_008e,32'h1110_008f}),
                        .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0040),
                        .data({32'h1211_0090,32'h1312_0091,32'h1413_0092,32'h1514_0093,32'h1615_0094,32'h1716_0095,32'h1817_0096,32'h1918_0097,
                               32'h1a19_0098,32'h1b1a_0099,32'h1c1b_009a,32'h1d1c_009b,32'h1e1d_009c,32'h1f1e_009d,32'h201f_009e,32'h2120_009f}),
                        .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        //cyclic1
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0100),
                        .data({32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                               32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                        .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0140),
                        .data({32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,
                               32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0,32'h0000_03a0}),
                        .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
    endtask

endclass

class read_cyclic0_bram_seq extends simple_axi_master_base_sequence;
    `uvm_object_utils(read_cyclic0_bram_seq)
    function new(string name="read_cyclic0_bram_seq");
        super.new(name);
    endfunction

    virtual task body();
        simple_axi_seq_item trans_item;
        `uvm_create(trans_item);
        //cyclic0
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0400),
                        .data({32'h0001_0080,32'h0002_0081,32'h0003_0082,32'h0004_0083,32'h0005_0084,32'h0006_0085,32'h0007_0086,32'h0008_0087,
                               32'h0009_0088,32'h000a_0089,32'h000b_008a,32'h000c_008b,32'h000d_008c,32'h000e_008d,32'h000f_008e,32'h0010_008f}),
                        .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0440),
                        .data({32'h0011_0090,32'h0012_0091,32'h0013_0092,32'h0014_0093,32'h0015_0094,32'h0016_0095,32'h0017_0096,32'h0018_0097,
                               32'h0019_0098,32'h001a_0099,32'h001b_009a,32'h001c_009b,32'h001d_009c,32'h001e_009d,32'h001f_009e,32'h0020_009f}),
                        .length(15), .access_type(simple_axi_seq_item::READ), .resp_code(simple_axi_seq_item::OKAY));
    endtask

endclass
