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

void ics_if_tx(
    hls::stream<uint1_t> &ics_sig_o,
    hls::stream<uint1_t> &ics_sig_dir_o,//0:input,1:output
    hls::stream<uint8_t> &ics_char_i,//char
    ap_uint<10> bit_period_i
){
#pragma HLS INTERFACE axis off port=ics_char_i
#pragma HLS INTERFACE ap_ctrl_hs port=return
    ap_uint<8> ics_char_read;
    ap_uint<8> parity;
    ap_uint<10> bit_period;
    bit_period = bit_period_i;
    ics_char_read = ics_char_i.read();
    //parity
    parity = ics_char_read;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    ap_uint<10> ch = (ics_char_read & 0xff )* 2 + (parity & 0x01);//add start bit and parity bit.
    //wait for stop bit(1bit + extra 1bit)
    for(ap_uint<12> j=0; j < bit_period * 2; ++j){
#pragma HLS LOOP_FLATTEN off
        ics_sig_dir_o.write_nb(0);
    }
tx_loop_character:
    for(ap_uint<5>i = 0; i < 10; ++i){
tx_bit_period_loop:
        for(ap_uint<10> j = 0; j < bit_period; ++j){
            ics_sig_dir_o.write_nb(1);
            ics_sig_o.write_nb((ch >> (9 - i)) & 0x01);
        }
    }
    ics_sig_dir_o.write_nb(0);
    return;
}
