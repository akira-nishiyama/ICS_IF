`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/12/2020 03:54:05 PM
// Design Name: 
// Module Name: tb_ics_if
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

import axi_vip_pkg::*;
import sim_ics_if_axi_vip_0_0_pkg::*;

module tb_ics_if(

    );
    
reg ap_clk_0_0=0;
reg ap_rst_n_0_0=0;
wire [0:0]ics_sig_dir_o_V_V_din_0_0;
wire [0:0]ics_sig_i_V_V_0_0;
wire [0:0]ics_sig_o_V_V_din_0_0;
//vip agent
sim_ics_if_axi_vip_0_0_mst_t   agent;

//
/*************************************************************************************************
* Declare variables which will be used in API and parital randomization for transaction generation
* and data read back from driver.
*************************************************************************************************/
axi_transaction                                          wr_trans;            // Write transaction
axi_transaction                                          rd_trans;            // Read transaction
xil_axi_uint                                             mtestWID;            // Write ID  
xil_axi_ulong                                            mtestWADDR;          // Write ADDR  
xil_axi_len_t                                            mtestWBurstLength;   // Write Burst Length   
xil_axi_size_t                                           mtestWDataSize;      // Write SIZE  
xil_axi_burst_t                                          mtestWBurstType;     // Write Burst Type  
xil_axi_uint                                             mtestRID;            // Read ID  
xil_axi_ulong                                            mtestRADDR;          // Read ADDR  
xil_axi_len_t                                            mtestRBurstLength;   // Read Burst Length   
xil_axi_size_t                                           mtestRDataSize;      // Read SIZE  
xil_axi_burst_t                                          mtestRBurstType;     // Read Burst Type  

xil_axi_data_beat [255:0]                                mtestWUSER;         // Write user  
xil_axi_data_beat                                        mtestAWUSER;        // Write Awuser 
xil_axi_data_beat                                        mtestARUSER;        // Read Aruser 
/************************************************************************************************
* A burst can not cross 4KB address boundry for AXI4
* Maximum data bits = 4*1024*8 =32768
* Write Data Value for WRITE_BURST transaction
* Read Data Value for READ_BURST transaction
************************************************************************************************/
bit [32767:0]                                            mtestWData;         // Write Data 
bit[8*4096-1:0]                                          Rdatablock;        // Read data block
xil_axi_data_beat                                        Rdatabeat[];       // Read data beats
bit[8*4096-1:0]                                          Wdatablock;        // Write data block
xil_axi_data_beat                                        Wdatabeat[];       // Write data beats


sim_ics_if dut
   (.ap_clk_0_0(ap_clk_0_0),
    .ap_rst_n_0_0(ap_rst_n_0_0),
    .ics_sig_dir_o_V_V_din_0_0(ics_sig_dir_o_V_V_din_0_0),
    .ics_sig_i_V_V_0_0(ics_sig_i_V_V_0_0),
    .ics_sig_o_V_V_din_0_0(ics_sig_o_V_V_din_0_0));

always #5 ap_clk_0_0 <= ~ap_clk_0_0;

initial begin
    ap_rst_n_0_0 <= 0;
    repeat(20) @(posedge ap_clk_0_0);
    ap_rst_n_0_0 <= 1;
end

//main
initial begin
    //init
    agent = new("my vip agent", dut.axi_vip_0.inst.IF);
    agent.start_master();
    //write transaction
    mtestWID = $urandom_range(0,(1<<(0)-1)); 
    mtestWADDR = 0;
    mtestWBurstLength = 0;
    mtestWDataSize = xil_axi_size_t'(xil_clog2((32)/8));
    mtestWBurstType = XIL_AXI_BURST_TYPE_INCR;
    mtestWData = $urandom();
    //single write transaction filled in user inputs through API 
    single_write_transaction_api("single write with api",
                                 .id(mtestWID),
                                 .addr(mtestWADDR),
                                 .len(mtestWBurstLength), 
                                 .size(mtestWDataSize),
                                 .burst(mtestWBurstType),
                                 .wuser(mtestWUSER),
                                 .awuser(mtestAWUSER), 
                                 .data(mtestWData)
                                 );
    #1000;
    $finish();
end

//tasks
/************************************************************************************************
*  task single_write_transaction_api is to create a single write transaction, fill in transaction 
*  by using APIs and send it to write driver.
*   1. declare write transction
*   2. Create the write transaction
*   3. set addr, burst,ID,length,size by calling set_write_cmd(addr, burst,ID,length,size), 
*   4. set prot.lock, cache,region and qos
*   5. set beats
*   6. set AWUSER if AWUSER_WIDH is bigger than 0
*   7. set WUSER if WUSR_WIDTH is bigger than 0
*************************************************************************************************/

task automatic single_write_transaction_api ( 
                            input string                     name ="single_write",
                            input xil_axi_uint               id =0, 
                            input xil_axi_ulong              addr =0,
                            input xil_axi_len_t              len =0, 
                            input xil_axi_size_t             size =xil_axi_size_t'(xil_clog2((32)/8)),
                            input xil_axi_burst_t            burst =XIL_AXI_BURST_TYPE_INCR,
                            input xil_axi_lock_t             lock = XIL_AXI_ALOCK_NOLOCK,
                            input xil_axi_cache_t            cache =3,
                            input xil_axi_prot_t             prot =0,
                            input xil_axi_region_t           region =0,
                            input xil_axi_qos_t              qos =0,
                            input xil_axi_data_beat [255:0]  wuser =0, 
                            input xil_axi_data_beat          awuser =0,
                            input bit [32767:0]              data =0
                                            );
    axi_transaction                               wr_trans;
    wr_trans = agent.wr_driver.create_transaction(name);
    wr_trans.set_write_cmd(addr,burst,id,len,size);
    wr_trans.set_prot(prot);
    wr_trans.set_lock(lock);
    wr_trans.set_cache(cache);
    wr_trans.set_region(region);
    wr_trans.set_qos(qos);
        wr_trans.set_data_block(data);
        agent.wr_driver.send(wr_trans);   
endtask  : single_write_transaction_api

endmodule
