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
#include "ics_if_tx.h"

void print_result_character(
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
		//search startbit
		unsigned int skip_cnt = 0;
		while(ics_sig.read_nb(ics_sig_result)){
			++skip_cnt;
			if(ics_sig_result == 0){
//				std::cout << "search start bit(skip num):" << skip_cnt << std::endl;
				for(int i = 0; i < bit_period; ++i) ics_sig.read_nb(ics_sig_result);//skip start bit
				break;
			}
		}
		if(ics_sig.empty()) break;
		while(bit_cnt <= 7){
			ics_sig.read_nb(ics_sig_result);
			if(ics_sig_result == 1) {
				++cnt_high;
			}else{
				++cnt_low;
			}
			++cnt;
			if(cnt >= bit_period){
				if(cnt_high > cnt_low){
					data[size] = data[size] * 2 + 1;
//					std::cout << std::setw(2) << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
				}else{
					data[size] = data[size] * 2;
//					std::cout << std:: setw(2) << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
				}
				cnt_high = 0;
				cnt_low = 0;
				cnt = 0;
				++bit_cnt;
			}
		}
//		std::cout << "byte:" << std::setw(2) << std::setfill('0') << std::hex << data[size] << std::endl;
		bit_cnt = 0;
		++size;
		data[size] = 0;
	}
	std::cout << "character:";
	for(int i = 0; i < size; ++i){
		std::cout << std::setw(2) << std::setfill('0') << std::hex << data[i] << ' ';
	}
	std::cout << std::endl;
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
				std::cout << "cnt_low=" << cnt_low << std::endl;
			}
			++cnt;
			if(cnt >= bit_period){
				if(cnt_high > cnt_low){
					data[size] = data[size] * 2 + 1;
					std::cout << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
				}else{
					data[size] = data[size] * 2;
					std::cout << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
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

int main(void){
	ap_uint<10> bit_period = 87;
	hls::stream<uint1_t> direction;
	hls::stream<uint9_t> ics_char;
	hls::stream<uint1_t> ics_sig_o;

	//initialize ics_sig_i
	//cyclic0
	//cyclic0 servo_index0ics_char
	ics_char.write(0x180);//loopback
	ics_char.write(0x103);//loopback
	ics_char.write(0x002);//loopback
	//cyclic0 servo_index1
	ics_char.write(0x181);//loopback
	ics_char.write(0x106);//loopback
	ics_char.write(0x005);//loopback
	//cyclic0 servo_index2
	ics_char.write(0x182);//loopback
	ics_char.write(0x109);//loopback
	ics_char.write(0x008);//loopback
	//cyclic0 servo_index31
	ics_char.write(0x19f);//loopback
	ics_char.write(0x10c);//loopback
	ics_char.write(0x00b);//loopback
	//cyclic1
	//cyclic1 servo_index0
	ics_char.write(0x1a0);//loopback
	ics_char.write(0x003);//loopback
	//cyclic1 servo_index1
	ics_char.write(0x1a1);//loopback
	ics_char.write(0x003);//loopback
	//cyclic1 servo_index2
	ics_char.write(0x1a2);//loopback
	ics_char.write(0x003);//loopback
	//cyclic1 servo_index31
	ics_char.write(0x1bf);//loopback
	ics_char.write(0x003);//loopback
	//cyclic2
	//cyclic2 servo_index0
	ics_char.write(0x1a0);//loopback
	ics_char.write(0x004);//loopback
	//cyclic2 servo_index1
	ics_char.write(0x1a1);//loopback
	ics_char.write(0x004);//loopback
	//cyclic2 servo_index2
	ics_char.write(0x1a2);//loopback
	ics_char.write(0x004);//loopback
	//cyclic2 servo_index31
	ics_char.write(0x1bf);//loopback
	ics_char.write(0x004);//loopback
	std::cout << "===============transmit test====================" << std::endl;
	for(int i = 0; i < 28; ++i){
		ics_if_tx(ics_sig_o, direction,ics_char,bit_period);
		print_result_character(ics_sig_o,bit_period);
		print_result_direction(direction,bit_period);
//		std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << ",dir:" << (int)direction.read() << std::endl;
	}
	return 0;
}
