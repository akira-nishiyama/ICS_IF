/************************************************************************
 * @file tb_ics_if_main.h
 * @brief ics interface core
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#ifndef ICS_IF_CORE_H
#define ICS_IF_CORE_H

#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>

#define DIR_I 0
#define DIR_O 1

#define COMM_KIND_CYCLIC0 1
#define COMM_KIND_CYCLIC1 2
#define COMM_KIND_CYCLIC2 4
#define COMM_KIND_ONDEMAND 8

#define TX_BUF_CYCLIC0_OFFSET 0
#define TX_BUF_CYCLIC1_OFFSET 32
#define TX_BUF_CYCLIC2_OFFSET 64

#define RX_BUF_CYCLIC0_OFFSET 256
#define RX_BUF_CYCLIC1_OFFSET 288
#define RX_BUF_CYCLIC2_OFFSET 320

typedef ap_uint<1> uint1_t; // 1-bit user defined type
//typedef ap_uint<8> uint8_t; // 1-bit user defined type
typedef ap_uint<9> uint9_t; // 1-bit user defined type

void ics_if_main(
	//reg
	ap_uint<3> bit_period_config_i,
	ap_uint<6> number_of_servos_i,
	ap_uint<32> cyclic0_config_i,
	ap_uint<16> *cyclic0_interval_o,
	ap_uint<1>  *cyclic0_enable_o,
	ap_uint<32> cyclic1_config_i,
	ap_uint<16> *cyclic1_interval_o,
	ap_uint<1>  *cyclic1_enable_o,
	ap_uint<32> cyclic2_config_i,
	ap_uint<16> *cyclic2_interval_o,
	ap_uint<1>  *cyclic2_enable_o,
	ap_uint<8> *cmd_error_cnt_o,
	ap_uint<32> communication_memory[512],
	// ics_if_tx and ics_if_rx
	ap_uint<10> *bit_period_o,
	// ics_if_tx
	hls::stream<uint9_t> &ics_tx_char_o,
	// ics_if_rx
	hls::stream<uint8_t> &ics_rx_char_i,
	volatile ap_uint<1> *ics_rx_char_rst_o,
	//timer for cyclic 0
	hls::stream<uint1_t> &cyclic0_start_i,
	//timer for cyclic 1
	hls::stream<uint1_t> &cyclic1_start_i,
	//timer for cyclic 2
	hls::stream<uint1_t> &cyclic2_start_i
);

#endif
