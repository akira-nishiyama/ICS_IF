`timescale 1ns/1ps

// tb_ics_if_uvm.sv
//      This file implements the testbench for ics_if.
//
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

module tb_ics_if_uvm_multi_communication;
    `include "uvm_macros.svh"
    import uvm_pkg::*;
    import simple_axi_uvm_pkg::*;
    import tb_ics_if_uvm_test_pkg::*;

    //signal declaration
    logic clk, rstz;
    wire ics_sig;

    //axi4-lite interface
    simple_axi_if sif(
        .aclk(clk),
        .arstn(rstz)
    );

    //axi4-bram interface
    simple_axi_if mem_if(
        .aclk(clk),
        .arstn(rstz)
    );

    //uart interface
    simple_uart_if uart_if();

    ics_if dut(.ap_clk_0(clk), .ap_rst_n_0(rstz),
        //axi4-lite interface
        .s_axi_for_reg_araddr(sif.axi_araddr),
        .s_axi_for_reg_arready(sif.axi_arready),
        .s_axi_for_reg_arvalid(sif.axi_arvalid),
        .s_axi_for_reg_awaddr(sif.axi_awaddr),
        .s_axi_for_reg_awready(sif.axi_awready),
        .s_axi_for_reg_awvalid(sif.axi_awvalid),
        .s_axi_for_reg_bready(sif.axi_bready),
        .s_axi_for_reg_bresp(sif.axi_bresp),
        .s_axi_for_reg_bvalid(sif.axi_bvalid),
        .s_axi_for_reg_rdata(sif.axi_rdata),
        .s_axi_for_reg_rready(sif.axi_rready),
        .s_axi_for_reg_rresp(sif.axi_rresp),
        .s_axi_for_reg_rvalid(sif.axi_rvalid),
        .s_axi_for_reg_wdata(sif.axi_wdata),
        .s_axi_for_reg_wready(sif.axi_wready),
        .s_axi_for_reg_wstrb(sif.axi_wstrb),
        .s_axi_for_reg_wvalid(sif.axi_wvalid),
        //axi-bram interface
        .S_AXI_for_bram_araddr(mem_if.axi_araddr),
        .S_AXI_for_bram_arburst(mem_if.axi_arburst),
        .S_AXI_for_bram_arcache(mem_if.axi_arcache),
        .S_AXI_for_bram_arlen(mem_if.axi_arlen),
        .S_AXI_for_bram_arlock(mem_if.axi_arlock),
        .S_AXI_for_bram_arprot(mem_if.axi_arprot),
        .S_AXI_for_bram_arready(mem_if.axi_arready),
        .S_AXI_for_bram_arsize(mem_if.axi_arsize),
        .S_AXI_for_bram_arvalid(mem_if.axi_arvalid),
        .S_AXI_for_bram_awaddr(mem_if.axi_awaddr),
        .S_AXI_for_bram_awburst(mem_if.axi_awburst),
        .S_AXI_for_bram_awcache(mem_if.axi_awcache),
        .S_AXI_for_bram_awlen(mem_if.axi_awlen),
        .S_AXI_for_bram_awlock(mem_if.axi_awlock),
        .S_AXI_for_bram_awprot(mem_if.axi_awprot),
        .S_AXI_for_bram_awready(mem_if.axi_awready),
        .S_AXI_for_bram_awsize(mem_if.axi_awsize),
        .S_AXI_for_bram_awvalid(mem_if.axi_awvalid),
        .S_AXI_for_bram_bready(mem_if.axi_bready),
        .S_AXI_for_bram_bresp(mem_if.axi_bresp),
        .S_AXI_for_bram_bvalid(mem_if.axi_bvalid),
        .S_AXI_for_bram_rdata(mem_if.axi_rdata),
        .S_AXI_for_bram_rlast(mem_if.axi_rlast),
        .S_AXI_for_bram_rready(mem_if.axi_rready),
        .S_AXI_for_bram_rresp(mem_if.axi_rresp),
        .S_AXI_for_bram_rvalid(mem_if.axi_rvalid),
        .S_AXI_for_bram_wdata(mem_if.axi_wdata),
        .S_AXI_for_bram_wlast(mem_if.axi_wlast),
        .S_AXI_for_bram_wready(mem_if.axi_wready),
        .S_AXI_for_bram_wstrb(mem_if.axi_wstrb),
        .S_AXI_for_bram_wvalid(mem_if.axi_wvalid),
        //ics signal
        .ics_sig(ics_sig));

    initial begin
        fork
            begin
                clk = 1'b1;
                #100;
                forever #5 clk = ~clk;
            end
            begin
                rstz = 1'b0;
                #500;
                rstz = 1'b1;
            end
        join
    end
    assign sif.axi_rlast = 1;

    pullup(ics_sig);
    assign ics_sig = (uart_if.piso === 0) ? 1'b0 : 1'bz;
    //assign ics_sig = (ics_sig === 1'b1 ) ? uart_if.piso : ics_sig;
    assign uart_if.posi = ics_sig;

    initial begin
        set_global_timeout(200000000ns);
        `uvm_info("info", "Hello World from initial block", UVM_LOW)
        uvm_config_db#(virtual simple_axi_if)::set(uvm_root::get(), "uvm_test_top.tb_ics_if_uvm_env.agent.*", "vif", sif);
        uvm_config_db#(virtual simple_axi_if)::set(uvm_root::get(), "uvm_test_top.tb_ics_if_uvm_env.agent_mem.*", "vif", mem_if);
        uvm_config_db#(bit)::set(uvm_root::get(), "*", "axi_transaction_mode", 0);
        uvm_config_db#(virtual simple_uart_if)::set(uvm_root::get(),"uvm_test_top.tb_ics_if_uvm_env.ics_env.*", "vif",uart_if);
        run_test("ics_if_multi_communication_test");
    end

endmodule
