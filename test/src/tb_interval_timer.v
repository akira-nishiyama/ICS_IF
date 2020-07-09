`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/04/2020 12:39:35 PM
// Design Name: 
// Module Name: tb_interval_timer
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


module tb_interval_timer();

reg ap_clk = 0;
reg ap_rst_n;
reg[15:0] interval_i = 10;
reg interrupt_en;
wire interrupt_o;

interval_timer uut(
    ap_clk,
    ap_rst_n,
    interval_i,
    interrupt_en,
    interrupt_o
);

always #5 ap_clk <= ~ap_clk;

initial begin
    ap_rst_n <= 0;
    repeat(20) @(posedge ap_clk);
    ap_rst_n <= 1;
end

initial begin
    interrupt_en <= 0;
    repeat(65536*11) @(posedge ap_clk);
    interrupt_en <= 1;
end



endmodule
