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
	hls::stream<uint1_t> &ics_sig_dir_o,
	hls::stream<uint9_t> &ics_char_i,//{direction_flag, char}
	ap_uint<10> bit_period_i
){
#pragma HLS INTERFACE ap_ctrl_hs port=return
#pragma HLS INTERFACE axis register forward port=ics_char_i
#pragma HLS PIPELINE II=1
	ap_uint<9> ics_char_read;
	ap_uint<1> dir;
	ap_uint<1> now;
	ap_uint<1> next;

	ics_char_read = ics_char_i.read();
	ap_uint<10> ch = (ics_char_read & 0xff )* 2 + 1;//add start bit and stop bit
	dir = ics_char_read >> 8;
	#ifndef __SYNTHESIS__
		std::cout << "[ics_if_tx dir]:" << std::hex << dir << std::endl;
	#endif
tx_loop_character:
	for(ap_uint<10>i = 0; i < 10; ++i){
		#pragma HLS PIPELINE
		#pragma HLS LOOP_FLATTEN off
tx_bit_period_loop:
		for(ap_uint<10> j = 0; j < bit_period_i; ++j){
			#pragma HLS LOOP_FLATTEN off
			ics_sig_dir_o.write_nb(0);
			ics_sig_o.write_nb((ch >> (9 - i)) & 0x01);
		}
	}
	ics_sig_dir_o.write_nb(dir);
	return;
}
