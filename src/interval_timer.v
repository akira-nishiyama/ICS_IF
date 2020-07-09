`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/04/2020 09:39:05 AM
// Design Name: 
// Module Name: interval_timer
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


module interval_timer(
    input       ap_clk,
    input       ap_rst_n,
    input[15:0] interval_i,
    input       interrupt_en,
    output      interrupt_o
    );

wire[47:0] p;//for debug
wire rst;
wire interrupt;

// DSP48E1: 48-bit Multi-Functional Arithmetic Block
// 7 Series
// Xilinx HDL Libraries Guide, version 2013.4
DSP48E1 #(
// Feature Control Attributes: Data Path Selection
.A_INPUT("DIRECT"),
.B_INPUT("DIRECT"),
// Selects A input source, "DIRECT" (A port) or "CASCADE" (ACIN port)
// Selects B input source, "DIRECT" (B port) or "CASCADE" (BCIN port)
.USE_DPORT("FALSE"),
// Select D port usage (TRUE or FALSE)
.USE_MULT("MULTIPLY"),
// Select multiplier usage ("MULTIPLY", "DYNAMIC", or "NONE")
.USE_SIMD("ONE48"),
// SIMD selection ("ONE48", "TWO24", "FOUR12")
// Pattern Detector Attributes: Pattern Detection Configuration
.AUTORESET_PATDET("RESET_MATCH"),
// "NO_RESET", "RESET_MATCH", "RESET_NOT_MATCH"
.MASK(48'hffff00000000),
// 48-bit mask value for pattern detect (1=ignore)
.PATTERN(48'h000000000000),
// 48-bit pattern match for pattern detect
.SEL_MASK("MASK"),
// "C", "MASK", "ROUNDING_MODE1", "ROUNDING_MODE2"
.SEL_PATTERN("C"),
// Select pattern value ("PATTERN" or "C")
.USE_PATTERN_DETECT("PATDET"), // Enable pattern detect ("PATDET" or "NO_PATDET")
// Register Control Attributes: Pipeline Register Configuration
.ACASCREG(1),
// Number of pipeline stages between A/ACIN and ACOUT (0, 1 or 2)
.ADREG(1),
// Number of pipeline stages for pre-adder (0 or 1)
.ALUMODEREG(1),
// Number of pipeline stages for ALUMODE (0 or 1)
.AREG(1),
// Number of pipeline stages for A (0, 1 or 2)
.BCASCREG(1),
// Number of pipeline stages between B/BCIN and BCOUT (0, 1 or 2)
.BREG(1),
// Number of pipeline stages for B (0, 1 or 2)
.CARRYINREG(1),
// Number of pipeline stages for CARRYIN (0 or 1)
.CARRYINSELREG(1),
// Number of pipeline stages for CARRYINSEL (0 or 1)
.CREG(1),
// Number of pipeline stages for C (0 or 1)
.DREG(1),
// Number of pipeline stages for D (0 or 1)
.INMODEREG(1),
// Number of pipeline stages for INMODE (0 or 1)
.MREG(1),
// Number of multiplier pipeline stages (0 or 1)
.OPMODEREG(1),
// Number of pipeline stages for OPMODE (0 or 1)
.PREG(1)
// Number of pipeline stages for P (0 or 1)
)
DSP48E1_inst (
// Cascade: 30-bit (each) output: Cascade Ports
.ACOUT(),
// 30-bit output: A port cascade output
.BCOUT(),
// 18-bit output: B port cascade output
.CARRYCASCOUT(),
// 1-bit output: Cascade carry output
.MULTSIGNOUT(),
// 1-bit output: Multiplier sign cascade output
.PCOUT(PCOUT),
// 48-bit output: Cascade output
// Control: 1-bit (each) output: Control Inputs/Status Bits
.OVERFLOW(),
// 1-bit output: Overflow in add/acc output
.PATTERNBDETECT(), // 1-bit output: Pattern bar detect output
.PATTERNDETECT(interrupt),
// 1-bit output: Pattern detect output
.UNDERFLOW(),
// 1-bit output: Underflow in add/acc output
// Data: 4-bit (each) output: Data Ports
.CARRYOUT(),
// 4-bit output: Carry output
.P(p),
// 48-bit output: Primary data output
// Cascade: 30-bit (each) input: Cascade Ports
.ACIN(0),
// 30-bit input: A cascade data input
.BCIN(0),
// 18-bit input: B cascade input
.CARRYCASCIN(0),
// 1-bit input: Cascade carry input
.MULTSIGNIN(0),
// 1-bit input: Multiplier sign input
.PCIN(0),
// 48-bit input: P cascade input
// Control: 4-bit (each) input: Control Inputs/Status Bits
.ALUMODE(0),
// 4-bit input: ALU control input. X(A:B) + Y(0) + Z(P) + CIN(0)
.CARRYINSEL(0),
// 3-bit input: Carry select input
.CLK(ap_clk),
// 1-bit input: Clock input
.INMODE(0),
// 5-bit input: INMODE control input
.OPMODE(7'b0100011),
// 7-bit input: Operation mode input,Z=P,Y=0,X=A:B,
// Data: 30-bit (each) input: Data Ports
.A(30'h0),
// 30-bit input: A data input,upper 30bit.
.B(18'h1),
// 18-bit input: B data input,lower 18bit. set 1 for count up.
.C({16'h0,interval_i,16'h0}),
// 48-bit input: C data input. use for patterndet
.CARRYIN(0),
// 1-bit input: Carry input signal
.D(25'h0),
// 25-bit input: D data input
// Reset/Clock Enable: 1-bit (each) input: Reset/Clock Enable Inputs
.CEA1(0),
// 1-bit input: Clock enable input for 1st stage AREG
.CEA2(1),
// 1-bit input: Clock enable input for 2nd stage AREG
.CEAD(0),
// 1-bit input: Clock enable input for ADREG
.CEALUMODE(1),
// 1-bit input: Clock enable input for ALUMODE
.CEB1(0),
// 1-bit input: Clock enable input for 1st stage BREG
.CEB2(1),
// 1-bit input: Clock enable input for 2nd stage BREG
.CEC(1),
// 1-bit input: Clock enable input for CREG
.CECARRYIN(0),
// 1-bit input: Clock enable input for CARRYINREG
.CECTRL(1),
// 1-bit input: Clock enable input for OPMODEREG and CARRYINSELREG
.CED(0),
// 1-bit input: Clock enable input for DREG
.CEINMODE(1),
// 1-bit input: Clock enable input for INMODEREG
.CEM(0),
// 1-bit input: Clock enable input for MREG
.CEP(1),
// 1-bit input: Clock enable input for PREG
.RSTA(rst),
// 1-bit input: Reset input for AREG
.RSTALLCARRYIN(rst),
// 1-bit input: Reset input for CARRYINREG
.RSTALUMODE(rst),
// 1-bit input: Reset input for ALUMODEREG
.RSTB(rst),
// 1-bit input: Reset input for BREG
.RSTC(rst),
// 1-bit input: Reset input for CREG
.RSTCTRL(rst),
// 1-bit input: Reset input for OPMODEREG and CARRYINSELREG
.RSTD(rst),
// 1-bit input: Reset input for DREG and ADREG
.RSTINMODE(rst),
// 1-bit input: Reset input for INMODEREG
.RSTM(rst),
// 1-bit input: Reset input for MREG
.RSTP(rst)
// 1-bit input: Reset input for PREG
);

assign rst = ~ap_rst_n;

assign interrupt_o = interrupt & interrupt_en;

endmodule
