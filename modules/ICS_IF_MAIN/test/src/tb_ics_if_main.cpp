/************************************************************************
 * @file tb_ics_if_core.cpp
 * @brief testbench of ics_if_core.main
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "ics_if_main.h"

void print_result_character(
		hls::stream<uint9_t> &ics_sig
	){
	ap_uint<9> reg;
	std::cout << "character:" << std::endl;
	while(!ics_sig.empty()){
		ics_sig.read_nb(reg);
		std::cout << std::setw(3) << std::setfill('0') << std::hex << reg << std::endl;
	}
}

void print_result_direction(
		hls::stream<uint1_t> &ics_sig,
		ap_int<10> bit_period
	){
	ap_uint<1> ics_sig_result;
	unsigned int cnt = 0;
	unsigned int cnt_low = 0;
	unsigned int cnt_high = 0;
	unsigned char bit_cnt = 0;
	unsigned int data[64] = {};
	unsigned char size = 0;
	while(!ics_sig.empty()){
		while(ics_sig.read_nb(ics_sig_result)){
			if(ics_sig_result == 1) {
				++cnt_high;
			}else{
				++cnt_low;
			}
			++cnt;
			if(cnt >= bit_period){
				if(cnt_high > cnt_low){
					data[size] = data[size] * 2 + 1;
//					std::cout << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
				}else{
					data[size] = data[size] * 2;
//					std::cout << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
				}
				cnt_high = 0;
				cnt_low = 0;
				cnt = 0;
				if(bit_cnt >= 9){
					bit_cnt = 0;
					++size;
					data[size] = 0;
				}else{
					++bit_cnt;
				}
			}
		}
	}
	std::cout << "direction:";
	for(int i = 0; i < size; ++i){
		std::cout << std::setw(3) << std::setfill('0') << std::hex << data[i] << ' ';
	}
	std::cout << std::endl;
}

void input_stream(
	hls::stream<uint1_t> &ics_sig,
	unsigned char ch,
	ap_int<10> bit_period
){
	ap_uint<10> tx_ch = (ch & 0x0ff) * 2 + 1;//add start bit and stop bit.
	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < bit_period; ++j){
			ics_sig.write_nb((tx_ch >> (10 - i - 1) & 0x01));
		}
	}
}

void input_idle(
	hls::stream<uint1_t> &ics_sig,
	int count
){
	for(int i = 0; i < count; ++i){
		ics_sig.write_nb(1);
	}
}

void dump_memory(
	ap_uint<32> communication_memory[512]
){
	int val;
	for(int i = 0; i < 512; ++i){
		val = communication_memory[i];
		std::cout << "mem[" << std::setw(3) << std::setfill('0') << std::hex << i * 4 + 0x800 << "]:";
		std::cout << std::setw(8) << std::setfill('0') << std::hex << val << std::endl;
	}
	std::cout << std::resetiosflags(std::ios_base::floatfield);
}

int main(void){
	ap_uint<10> bit_period;
	ap_uint<5> number_of_servos = 1;
	ap_uint<8> cmd_error_cnt;
	ap_uint<32> communication_memory[512] = {};
	ap_uint<8> ics_char=0;
	hls::stream<uint9_t> ics_tx_char;
	hls::stream<uint8_t> ics_rx_char;
//	std::cout << "Hello iostream" << std::endl;
	hls::stream<uint1_t>  cyclic0_start;
	hls::stream<uint1_t>  cyclic1_start;
	hls::stream<uint1_t>  cyclic2_start;
	volatile ap_uint<10> dummy = 0;
	volatile ap_uint<1> ics_rx_char_rst;

	//initialize communication memory
		for(int i = 0; i < 512; ++i){
//			communication_memory[i] =	(((i * 4 + 4) & 0xff) << 24) +
//										(((i * 4 + 3) & 0xff) << 16) +
//										(((i * 4 + 2) & 0xff) << 8 ) +
//										 ((i * 4 + 1) & 0xff);
			communication_memory[i] = 0;
		}
	//cyclic0
	for(int i = 0; i < 32; ++i){
		communication_memory[i] = (i + ((i + 1) << 8) + ((i + 2) << 16)) | 0x80;
	}
//	//cyclic1
//	communication_memory[32] = 0x000003a0;
//	communication_memory[33] = 0x000003a1;
//	communication_memory[34] = 0x000003a2;
//	communication_memory[63] = 0x000003bf;
//	//cyclic2
//	communication_memory[64] = 0x000004a0;
//	communication_memory[65] = 0x000004a1;
//	communication_memory[66] = 0x000004a2;
//	communication_memory[95] = 0x000004bf;


	//initialize ics_sig_i
	//cyclic0
	//cyclic0 servo_index0
	for(int i = 0; i < 32; ++i){
		ics_rx_char.write_nb(i);
		ics_rx_char.write_nb((i+2) << 1); //upper position
		ics_rx_char.write_nb(i+1); //lower position
	}
//	//cyclic1
//	//cyclic1 servo_index0
//	input_stream(ics_sig_i,0xa0,bit_period);//loopback
//	input_stream(ics_sig_i,0x03,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x20,bit_period);//receive
//	input_stream(ics_sig_i,0x03,bit_period);//receive
//	input_stream(ics_sig_i,0x44,bit_period);//receive
//	//cyclic1 servo_index1
//	input_stream(ics_sig_i,0xa1,bit_period);//loopback
//	input_stream(ics_sig_i,0x03,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x21,bit_period);//receive
//	input_stream(ics_sig_i,0x03,bit_period);//receive
//	input_stream(ics_sig_i,0x45,bit_period);//receive
//	//cyclic1 servo_index2
//	input_stream(ics_sig_i,0xa2,bit_period);//loopback
//	input_stream(ics_sig_i,0x03,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x22,bit_period);//receive
//	input_stream(ics_sig_i,0x03,bit_period);//receive
//	input_stream(ics_sig_i,0x46,bit_period);//receive
//	//cyclic1 servo_index31
//	input_stream(ics_sig_i,0xbf,bit_period);//loopback
//	input_stream(ics_sig_i,0x03,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x3f,bit_period);//receive
//	input_stream(ics_sig_i,0x03,bit_period);//receive
//	input_stream(ics_sig_i,0x47,bit_period);//receive
//	//cyclic2
//	//cyclic2 servo_index0
//	input_stream(ics_sig_i,0xa0,bit_period);//loopback
//	input_stream(ics_sig_i,0x04,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x20,bit_period);//receive
//	input_stream(ics_sig_i,0x04,bit_period);//receive
//	input_stream(ics_sig_i,0x48,bit_period);//receive
//	//cyclic2 servo_index1
//	input_stream(ics_sig_i,0xa1,bit_period);//loopback
//	input_stream(ics_sig_i,0x04,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x21,bit_period);//receive
//	input_stream(ics_sig_i,0x04,bit_period);//receive
//	input_stream(ics_sig_i,0x49,bit_period);//receive
//	//cyclic2 servo_index2
//	input_stream(ics_sig_i,0xa2,bit_period);//loopback
//	input_stream(ics_sig_i,0x04,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x22,bit_period);//receive
//	input_stream(ics_sig_i,0x04,bit_period);//receive
//	input_stream(ics_sig_i,0x4a,bit_period);//receive
//	//cyclic2 servo_index31
//	input_stream(ics_sig_i,0xbf,bit_period);//loopback
//	input_stream(ics_sig_i,0x04,bit_period);//loopback
//	input_idle(ics_sig_i,100);
//	input_stream(ics_sig_i,0x3f,bit_period);//receive
//	input_stream(ics_sig_i,0x04,bit_period);//receive
//	input_stream(ics_sig_i,0x4b,bit_period);//receive

	cyclic0_start.write_nb(1);

	std::cout << "===============cyclic0 test====================" << std::endl;

	ap_uint<32> cyclic0_config, cyclic1_config,cyclic2_config;
	ap_uint<1> cyclic0_enable, cyclic1_enable,cyclic2_enable;
	cyclic0_config = 0x80000032;
	cyclic1_config = 0x80000032;
	cyclic2_config = 0x800003e8;
	ap_uint<16> cyclic0_interval, cyclic1_interval, cyclic2_interval;
	ics_if_main(
		1,//ap_uint<3> bit_period_config_i,
		32,//ap_uint<6> number_of_servos_i,
		cyclic0_config,
		&cyclic0_interval,
		&cyclic0_enable,
		cyclic1_config,
		&cyclic1_interval,
		&cyclic1_enable,
		cyclic2_config,
		&cyclic2_interval,
		&cyclic2_enable,
		&cmd_error_cnt,//ap_uint<8> cmd_error_cnt_o,
		communication_memory,
		// ics_if_tx and ics_if_rx
		&bit_period,
		// ics_if_tx
		ics_tx_char,//hls::stream<uint9_t> &ics_tx_char_o,
		// ics_if_rx
		ics_rx_char,//hls::stream<uint8_t> &ics_rx_char_i,
		&ics_rx_char_rst,
		//timer for cyclic 0
		cyclic0_start,//hls::stream<uint1_t> &cyclic0_start_i,
		//timer for cyclic 1
		cyclic1_start,//hls::stream<uint1_t> &cyclic1_start_i,
		//timer for cyclic 2
		cyclic2_start//hls::stream<uint1_t> &cyclic2_start_i
	);

	ics_if_main(
		1,//ap_uint<3> bit_period_config_i,
		32,//ap_uint<6> number_of_servos_i,
		cyclic0_config,
		&cyclic0_interval,
		&cyclic0_enable,
		cyclic1_config,
		&cyclic1_interval,
		&cyclic1_enable,
		cyclic2_config,
		&cyclic2_interval,
		&cyclic2_enable,
		&cmd_error_cnt,//ap_uint<8> cmd_error_cnt_o,
		communication_memory,
		// ics_if_tx and ics_if_rx
		&bit_period,
		// ics_if_tx
		ics_tx_char,//hls::stream<uint9_t> &ics_tx_char_o,
		// ics_if_rx
		ics_rx_char,//hls::stream<uint8_t> &ics_rx_char_i,
		&ics_rx_char_rst,
		//timer for cyclic 0
		cyclic0_start,//hls::stream<uint1_t> &cyclic0_start_i,
		//timer for cyclic 1
		cyclic1_start,//hls::stream<uint1_t> &cyclic1_start_i,
		//timer for cyclic 2
		cyclic2_start//hls::stream<uint1_t> &cyclic2_start_i
	);


	dump_memory(communication_memory);
	print_result_character(ics_tx_char);
	std::cout << "cmd_err:" << cmd_error_cnt << std::endl;

	return 0;
}
