/************************************************************************
 * @file ics_if_tx.cpp
 * @brief ics interface tx which has serialize function.
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#include "ics_if_tx.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

void ics_if_tx(
    hls::stream<uint1_t> &ics_sig_o,
    hls::stream<uint1_t> &ics_sig_dir_o,//0:input,1:output
    hls::stream<uint8_t> &ics_char_i,//char
    ap_uint<10> bit_period_i
){
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS INTERFACE axis register forward port=ics_char_i
#pragma HLS PIPELINE II=1
    ap_uint<8> ics_char_read;
    ap_uint<1> now;
    ap_uint<1> next;
    ap_uint<8> parity;
    ics_sig_dir_o.write_nb(0);
    ics_char_read = ics_char_i.read();
    //parity
    parity = ics_char_read;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    ap_uint<11> ch = (ics_char_read & 0xff )* 4 + (parity & 0x01)*2 + 1;//add start bit, parity bit and stop bit
tx_loop_character:
    for(ap_uint<5>i = 0; i < 11; ++i){
        #pragma HLS PIPELINE
        #pragma HLS LOOP_FLATTEN off
tx_bit_period_loop:
        for(ap_uint<10> j = 0; j < bit_period_i; ++j){
            #pragma HLS LOOP_FLATTEN off
            ics_sig_dir_o.write_nb(1);
            ics_sig_o.write_nb((ch >> (10 - i)) & 0x01);
        }
    }
    ics_sig_dir_o.write_nb(0);
    return;
}
