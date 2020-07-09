/************************************************************************
 * @file tb_ics_if_tx.cpp
 * @brief ics interface core
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#ifndef ICS_IF_RX_H
#define ICS_IF_RX_H

#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>

typedef ap_uint<1> uint1_t; // 1-bit user defined type
typedef ap_uint<9> uint9_t; // 8-bit user defined type

void ics_if_tx(
	hls::stream<uint1_t> &ics_sig_i,
	hls::stream<uint1_t> &ics_sig_dir_o,
	hls::stream<uint9_t> &ics_char_o,
	ap_uint<10> bit_period_i
);

#endif
