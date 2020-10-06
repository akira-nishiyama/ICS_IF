// dff_with_we.v
//      This file implements the dff with write enable.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

module dff_with_we#(parameter integer init_val = 1'b0) (
    input  wire clk,
    input  wire rst,
    input  wire d,
    input  wire we,
    output reg  q
);

always @(posedge clk) begin
    if(rst === 1'b1) begin
        q <= init_val[0];
    end else begin
        if(we === 1'b1) begin
            q <= d;
        end
    end
end

endmodule


