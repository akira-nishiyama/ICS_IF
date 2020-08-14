// tb_ics_if_uvm_test_pkg.svh
//      This file implements the test package for tb_ics_if.
//      
// Copyright (c) 2020 Akira Nishiyama.
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
//

package tb_ics_if_uvm_test_pkg;
    `include "uvm_macros.svh"
    import uvm_pkg::*;
    `include "gp_scoreboard.sv"
    import simple_axi_uvm_pkg::*;
    `include "tb_ics_if_uvm_env.svh"
    `include "tb_ics_if_uvm_sequence.svh"
    `include "tb_ics_if_uvm_test.svh"

endpackage
