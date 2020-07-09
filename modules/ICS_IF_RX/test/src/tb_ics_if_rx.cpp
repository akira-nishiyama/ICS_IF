/************************************************************************
 * @file tb_ics_if_rx.cpp
 * @brief testbench of ics_if_rx.
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
#include "ics_if_rx.h"

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

int main(void){
	ap_uint<10> bit_period = 87;
	ap_uint<5> number_of_servos = 1;
	hls::stream<uint8_t> ics_char;
	hls::stream<uint1_t> ics_sig_i;


	//initialize ics_sig_i
	//cyclic0
	//cyclic0 servo_index0
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x80,bit_period);//loopback
	input_stream(ics_sig_i,0x03,bit_period);//loopback
	input_stream(ics_sig_i,0x02,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x00,bit_period);//receive
	input_stream(ics_sig_i,0x04,bit_period);//receive
	input_stream(ics_sig_i,0x05,bit_period);//receive
	//cyclic0 servo_index1
	input_stream(ics_sig_i,0x81,bit_period);//loopback
	input_stream(ics_sig_i,0x06,bit_period);//loopback
	input_stream(ics_sig_i,0x05,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x01,bit_period);//receive
	input_stream(ics_sig_i,0x14,bit_period);//receive
	input_stream(ics_sig_i,0x15,bit_period);//receive
	//cyclic0 servo_index2
	input_stream(ics_sig_i,0x82,bit_period);//loopback
	input_stream(ics_sig_i,0x09,bit_period);//loopback
	input_stream(ics_sig_i,0x08,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x02,bit_period);//receive
	input_stream(ics_sig_i,0x24,bit_period);//receive
	input_stream(ics_sig_i,0x25,bit_period);//receive
	//cyclic0 servo_index31
	input_stream(ics_sig_i,0x9f,bit_period);//loopback
	input_stream(ics_sig_i,0x0c,bit_period);//loopback
	input_stream(ics_sig_i,0x0b,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x1f,bit_period);//receive
	input_stream(ics_sig_i,0x34,bit_period);//receive
	input_stream(ics_sig_i,0x35,bit_period);//receive
	//cyclic1
	//cyclic1 servo_index0
	input_stream(ics_sig_i,0xa0,bit_period);//loopback
	input_stream(ics_sig_i,0x03,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x20,bit_period);//receive
	input_stream(ics_sig_i,0x03,bit_period);//receive
	input_stream(ics_sig_i,0x44,bit_period);//receive
	//cyclic1 servo_index1
	input_stream(ics_sig_i,0xa1,bit_period);//loopback
	input_stream(ics_sig_i,0x03,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x21,bit_period);//receive
	input_stream(ics_sig_i,0x03,bit_period);//receive
	input_stream(ics_sig_i,0x45,bit_period);//receive
	//cyclic1 servo_index2
	input_stream(ics_sig_i,0xa2,bit_period);//loopback
	input_stream(ics_sig_i,0x03,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x22,bit_period);//receive
	input_stream(ics_sig_i,0x03,bit_period);//receive
	input_stream(ics_sig_i,0x46,bit_period);//receive
	//cyclic1 servo_index31
	input_stream(ics_sig_i,0xbf,bit_period);//loopback
	input_stream(ics_sig_i,0x03,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x3f,bit_period);//receive
	input_stream(ics_sig_i,0x03,bit_period);//receive
	input_stream(ics_sig_i,0x47,bit_period);//receive
	//cyclic2
	//cyclic2 servo_index0
	input_stream(ics_sig_i,0xa0,bit_period);//loopback
	input_stream(ics_sig_i,0x04,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x20,bit_period);//receive
	input_stream(ics_sig_i,0x04,bit_period);//receive
	input_stream(ics_sig_i,0x48,bit_period);//receive
	//cyclic2 servo_index1
	input_stream(ics_sig_i,0xa1,bit_period);//loopback
	input_stream(ics_sig_i,0x04,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x21,bit_period);//receive
	input_stream(ics_sig_i,0x04,bit_period);//receive
	input_stream(ics_sig_i,0x49,bit_period);//receive
	//cyclic2 servo_index2
	input_stream(ics_sig_i,0xa2,bit_period);//loopback
	input_stream(ics_sig_i,0x04,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x22,bit_period);//receive
	input_stream(ics_sig_i,0x04,bit_period);//receive
	input_stream(ics_sig_i,0x4a,bit_period);//receive
	//cyclic2 servo_index31
	input_stream(ics_sig_i,0xbf,bit_period);//loopback
	input_stream(ics_sig_i,0x04,bit_period);//loopback
	input_idle(ics_sig_i,100);
	input_stream(ics_sig_i,0x3f,bit_period);//receive
	input_stream(ics_sig_i,0x04,bit_period);//receive
	input_stream(ics_sig_i,0x4b,bit_period);//receive

	std::cout << "===============receive test====================" << std::endl;
	for(int i = 0; i < 64; ++i){
		while(ics_sig_i.read() == 1);//skip until start bit. it is done by external edge detect logic to assert ap_start.
		ics_if_rx(ics_sig_i,ics_char,bit_period);
		std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << std::endl;
	}
	return 0;
}
