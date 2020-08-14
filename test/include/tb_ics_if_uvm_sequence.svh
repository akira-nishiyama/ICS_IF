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
                        .data({32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,
                               32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080}),
                        .length(15), .access_type(simple_axi_seq_item::WRITE), .resp_code(simple_axi_seq_item::OKAY));
        transfer_item(  .trans_item(trans_item), .addr(32'h0000_0040),
                        .data({32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,
                               32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080,32'h0000_0080}),
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
