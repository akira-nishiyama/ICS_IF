/************************************************************************
 * @file ics_if_rx.cpp
 * @brief ics interface rx which has deserialize function.
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#include "ics_if_rx.h"

void ics_if_rx(
    hls::stream<uint1_t> &ics_sig_i,
    hls::stream<uint9_t> &ics_char_o,
    ap_uint<10> bit_period_i
){
#pragma HLS INTERFACE axis register forward port=ics_char_o
#pragma HLS INTERFACE ap_hs port=ics_sig_i
    uint8_t ch = 0;
    uint1_t pality = 0;
    uint1_t read_bit = 0;
    uint5_t ics_sig_low_cnt;
    uint10_t cnt;

    cnt = 0;
    ics_sig_low_cnt = 0;
search_start_bit:
    while(true){
        if(ics_sig_i.read() == 0){
            ++cnt;
            ++ics_sig_low_cnt;
            if(ics_sig_low_cnt > 10){
                break;
            }
        } else {
        	cnt = 0;
            ics_sig_low_cnt = 0;
        }
    }

    //skip startbit.
    while(cnt < bit_period_i){
    	ics_sig_i.read();
    	++cnt;
    }

rx_loop_character:
    for(ap_uint<5> j = 0; j < 8; ++j){
        #pragma HLS LOOP_FLATTEN off
    	read_bit = ics_rx_bit_judge(ics_sig_i,bit_period_i);
        ch = (ch << 1) + (uint8_t)read_bit;
        pality += read_bit;
    }
    //get pality
    if(pality == ics_rx_bit_judge(ics_sig_i,bit_period_i)) ics_char_o.write(ch);
    else ics_char_o.write(ch | 0x100);

    return;
}

ap_uint<1> ics_rx_bit_judge(
    hls::stream<uint1_t> &ics_sig_i,
    ap_uint<10> bit_period_i
){
    ap_uint<1> ics_sig_result;
    ap_uint<10> cnt = 0;
    ap_uint<10> cnt_high = 0;
    ap_uint<10> cnt_low = 0;
rx_loop_bit_judge:
    while(true){
        #pragma HLS LOOP_FLATTEN off
        ics_sig_i.read_nb(ics_sig_result);
        if(ics_sig_result == 1) {
            ++cnt_high;
        }else{
            ++cnt_low;
        }
        ++cnt;
        if(cnt >= bit_period_i){
            if(cnt_high > cnt_low){
                return 1;
            }else{
                return 0;
            }
            break;
        }
    }
}
