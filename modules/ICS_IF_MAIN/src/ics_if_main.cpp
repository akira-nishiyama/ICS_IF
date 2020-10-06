/************************************************************************
 * @file ics_if_main.cpp
 * @brief ics interface main.
 * @author Akira Nishiyama
 * @date 2020/06/20
 * @par History
 *      2020/06/20 initial version
 * @copyright Copyright (c) 2020 Akira Nishiyama
 * Released under the MIT license
 * https://opensource.org/licenses/mit-license.php
 ***********************************************************************/

#include "ics_if_main.h"

#ifndef __SYNTHESIS__
#include <iostream>
#endif

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
    hls::stream<uint8_t> &ics_tx_char_o,
    // ics_if_rx
    hls::stream<uint8_t> &ics_rx_char_i,
    volatile ap_uint<1> *ics_rx_char_rst_o,
    //timer for cyclic 0
    hls::stream<uint1_t> &cyclic0_start_i,
    //timer for cyclic 1
    hls::stream<uint1_t> &cyclic1_start_i,
    //timer for cyclic 2
    hls::stream<uint1_t> &cyclic2_start_i
    )
{
#pragma HLS INTERFACE s_axilite port=return bundle=slv0
#pragma HLS INTERFACE ap_fifo port=cyclic2_start_i
#pragma HLS INTERFACE ap_fifo port=cyclic1_start_i
#pragma HLS INTERFACE ap_fifo port=cyclic0_start_i
#pragma HLS INTERFACE ap_none port=ics_rx_char_rst_o
#pragma HLS INTERFACE ap_none port=bit_period_o
#pragma HLS INTERFACE ap_none port=cyclic0_interval_o
#pragma HLS INTERFACE ap_none port=cyclic1_interval_o
#pragma HLS INTERFACE ap_none port=cyclic2_interval_o
#pragma HLS INTERFACE ap_none port=cyclic0_enable_o
#pragma HLS INTERFACE ap_none port=cyclic1_enable_o
#pragma HLS INTERFACE ap_none port=cyclic2_enable_o
#pragma HLS INTERFACE axis register forward port=ics_tx_char_o
#pragma HLS INTERFACE axis register forward port=ics_rx_char_i
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE s_axilite port=bit_period_config_i offset=0x10 bundle=slv0
#pragma HLS INTERFACE s_axilite port=number_of_servos_i offset=0x18 bundle=slv0
#pragma HLS INTERFACE s_axilite port=cyclic0_config_i offset=0x20 bundle=slv0
#pragma HLS INTERFACE s_axilite port=cyclic1_config_i offset=0x28 bundle=slv0
#pragma HLS INTERFACE s_axilite port=cyclic2_config_i offset=0x30 bundle=slv0
#pragma HLS INTERFACE s_axilite port=cmd_error_cnt_o offset=0x38 bundle=slv0
#pragma HLS INTERFACE bram depth=512 port=communication_memory
//#pragma HLS dataflow
    ap_uint<1> cyclic0_command;
    ap_uint<1> cyclic1_command;
    ap_uint<1> cyclic2_command;
    volatile ap_uint<10> dummy = 0;
    ap_uint<17> rx_timeout_init;
    ap_uint<32> rx_word;
    uint8_t rx_char;
    ap_uint<8> cyclic0_rx_count;
    ap_uint<17> rx_timeout;
    ap_uint<1> cyclic0_config_enable;
    ap_uint<16> cyclic0_config_interval;
    ap_uint<1> cyclic1_config_enable;
    ap_uint<16> cyclic1_config_interval;
    ap_uint<1> cyclic2_config_enable;
    ap_uint<16> cyclic2_config_interval;
    static ap_uint<8> cmd_error_num;

    if(bit_period_config_i == 1){
        *bit_period_o = 868;
        rx_timeout_init = 104160;//868 clock * 3 byte * 10 bit (start+ascii+stop) * 2 (tx and rx) * 2
    }else if(bit_period_config_i == 2){
        *bit_period_o = 160;
        rx_timeout_init = 19200;//160 clock * 3 byte * 10 bit (start+ascii+stop) * 2 (tx and rx) * 2
    }else if(bit_period_config_i == 4){
        *bit_period_o = 87;
        rx_timeout_init = 10440;//87 clock * 3 byte * 10 bit (start+ascii+stop) * 2 (tx and rx) * 2
    }else{
        *bit_period_o = 868;
        rx_timeout_init = 104160;//868 clock * 3 byte * 10 bit (start+ascii+stop) * 2 (tx and rx) * 2
    }
    cyclic0_config_enable = cyclic0_config_i >> 31;
    *cyclic0_enable_o = cyclic0_config_enable;
    cyclic0_config_interval = cyclic0_config_i & 0xffff;
    cyclic1_config_enable = cyclic1_config_i >> 31;
    *cyclic1_enable_o = cyclic1_config_enable;
    cyclic1_config_interval = cyclic1_config_i & 0xffff;
    cyclic2_config_enable = cyclic2_config_i >> 31;
    *cyclic2_enable_o = cyclic2_config_enable;
    cyclic2_config_interval = cyclic2_config_i & 0xffff;
    *cyclic0_interval_o = cyclic0_config_interval;
    *cyclic1_interval_o = cyclic1_config_interval;
    *cyclic2_interval_o = cyclic2_config_interval;

    if(number_of_servos_i == 0 || number_of_servos_i > 32) return;//invalid config
    if(cyclic0_config_enable == 1 && cyclic0_start_i.read_nb(cyclic0_command)) {
        if(cyclic0_command == 1) {
            //get communication memory data
            *ics_rx_char_rst_o = 1;
            *ics_rx_char_rst_o = 0;
            ap_uint<3> rx_count;
            ap_uint<5> id;
            for(ap_uint<6> i = 0; i < number_of_servos_i; ++i) {
                rx_timeout = rx_timeout_init;
                //tx
                id = communication_memory[i] & 0x1f;
                for(ap_uint<2> j = 0; j < 3; ++j) {
                    ap_uint<8> ch = (communication_memory[i] >> (8 * j)) & 0xff;
                    //if(j == 2) ch = ch | 0x100;
                    ics_tx_char_o.write(ch);
                }
                //rx loop
                rx_count = 0;
                cyclic0_rx_count = ((communication_memory[i + RX_BUF_CYCLIC0_OFFSET] >> 8) + 1) & 0xff;
                while(true) {
                    if(!ics_rx_char_i.empty()){
                        ics_rx_char_i.read(rx_char);
                        if(rx_count == 0 && (((rx_char & 0xe0) != 0) || ((rx_char & 0x1f) != id))){
                            *cmd_error_cnt_o = ++cmd_error_num;
                            break;//cmd error
                        } else if(rx_count == 0) {
                            rx_word = rx_char + (cyclic0_rx_count << 8);
                        } else if(rx_count == 1){
                            rx_word = rx_word | (rx_char << 23);//position upper 7bit
                        } else if(rx_count == 2){
                            rx_word = rx_word | (rx_char << 16);//position lower 7bit
                            communication_memory[i + RX_BUF_CYCLIC0_OFFSET] = rx_word;
                            break;//end rx process
                        } else if(rx_count >= 3){
                            break; //error
                        }
                        ++rx_count;
                    }
                    --rx_timeout;
                    if(rx_timeout == 0) break;
                }
            }
        }
    }

//    else if(cyclic1_start_i.read_nb(cyclic1_command)){
//
//    }
    return;
}


//
//void ics_single_communication(
//    hls::stream<uint1_t> &ics_sig_o,
//    hls::stream<uint1_t> &ics_sig_dir_o,
//    hls::stream<uint1_t> &ics_sig_i,
//    ap_uint<32> communication_memory[512],
//    ap_uint<10> bit_period_i,
//    ap_uint<4> communication_kind,
//    ap_uint<5> servo_index,
//    volatile ap_uint<10> *dummy
//){
//    volatile ap_uint<32> rx_word = 0;
//    volatile ap_uint<8> rx_ch = 0;
//    ap_uint<8> rx_cmd = 0;
//    ap_uint<8> ch = 0;
//    ap_uint<8> rx_counter;
//    if((communication_kind & 0x01) == COMM_KIND_CYCLIC0) {//cyclic0
//        ap_uint<16> position = 0;
//        rx_counter = (communication_memory[servo_index + RX_BUF_CYCLIC0_OFFSET] >> 8) & 0xff;
//tx_loop_single_communication_cyclic0:
//        for(int i = 0; i < 3; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ch = ((communication_memory[servo_index + TX_BUF_CYCLIC0_OFFSET] >> 8 * i) & 0xff);
//            ics_tx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, ch, bit_period_i, dummy);
////            #ifndef __SYNTHESIS__
////                    int word = (ap_uint<8>)ch;
////                    std::cout << "[ics_single_communication tx_ch]:" << std::hex << word << std::endl;
////            #endif
//        }
//
//rx_loop_single_communication_cyclic0:
//        for(int i = 0; i < 3; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ics_rx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, &rx_ch, bit_period_i, dummy);
////            rx_word = (ap_uint<32>)rx_word + (((ap_uint<32>)rx_ch << (8 * i)));
//            if(i == 0) {
//                rx_cmd = (ap_uint<8>)rx_ch;
//            }else if(i == 1){
//                position = (ap_uint<16>)rx_ch << 7;
//            }else if(i == 2){
//                position = (ap_uint<16>)position + (ap_uint<16>)rx_ch;
//            }
////#ifndef __SYNTHESIS__
////            int c = (ap_uint<8>)rx_ch;
////            std::cout << "[ics_single_communication rx_ch]:" << std::hex << c << std::endl;
////            int word = (ap_uint<32>)rx_word;
////            std::cout << "[ics_single_communication rx_word]:" << std::hex << word << std::endl;
////#endif
//        }
////#ifndef __SYNTHESIS__
////        int word = (ap_uint<32>)rx_word;
////        std::cout << "[ics_single_communication rx_word2]:" << std::hex << word << std::endl;
////#endif
////        communication_memory[servo_index + RX_BUF_OFFSET] = rx_word;
//        communication_memory[servo_index + RX_BUF_CYCLIC0_OFFSET] = rx_cmd + ((rx_counter + 1) << 8) + ((ap_uint<32>)position << 16);
//    }else if((communication_kind & 0x02) == COMM_KIND_CYCLIC1) {//cyclic1
//        rx_counter = (communication_memory[servo_index + RX_BUF_CYCLIC1_OFFSET] >> 24) & 0xff;
//tx_loop_single_communication_cyclic1:
//        for(int i = 0; i < 2; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ch = ((communication_memory[servo_index + TX_BUF_CYCLIC1_OFFSET] >> 8 * i) & 0xff);
//            ics_tx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, ch, bit_period_i, dummy);
////            #ifndef __SYNTHESIS__
////                    int word = (ap_uint<8>)ch;
////                    std::cout << "[ics_single_communication tx_ch]:" << std::hex << word << std::endl;
////            #endif
//        }
//
//rx_loop_single_communication_cyclic1:
//        for(int i = 0; i < 3; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ics_rx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, &rx_ch, bit_period_i, dummy);
//            rx_word = (ap_uint<32>)rx_word + (((ap_uint<32>)rx_ch << (8 * i)));
////#ifndef __SYNTHESIS__
////            int c = (ap_uint<8>)rx_ch;
////            std::cout << "[ics_single_communication rx_ch]:" << std::hex << c << std::endl;
////            int word = (ap_uint<32>)rx_word;
////            std::cout << "[ics_single_communication rx_word]:" << std::hex << word << std::endl;
////#endif
//        }
//        communication_memory[servo_index + RX_BUF_CYCLIC1_OFFSET] = (ap_uint<32>)rx_word + ((rx_counter + 1) << 24);
//    }else if((communication_kind & 0x04) == COMM_KIND_CYCLIC2) {//cyclic2
//        rx_counter = (communication_memory[servo_index + RX_BUF_CYCLIC2_OFFSET] >> 24) & 0xff;
//tx_loop_single_communication_cyclic2:
//        for(int i = 0; i < 2; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ch = ((communication_memory[servo_index + TX_BUF_CYCLIC2_OFFSET] >> 8 * i) & 0xff);
//            ics_tx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, ch, bit_period_i, dummy);
////            #ifndef __SYNTHESIS__
////                    int word = (ap_uint<8>)ch;
////                    std::cout << "[ics_single_communicati
//            on tx_ch]:" << std::hex << word << std::endl;
////            #endif
//        }
//
//rx_loop_single_communication_cyclic2:
//        for(int i = 0; i < 3; ++i){
//            #pragma HLS LOOP_FLATTEN off
//            ics_rx_char(ics_sig_o, ics_sig_dir_o, ics_sig_i, &rx_ch, bit_period_i, dummy);
//            rx_word = (ap_uint<32>)rx_word + (((ap_uint<32>)rx_ch << (8 * i)));
////#ifndef __SYNTHESIS__
////            int c = (ap_uint<8>)rx_ch;
////            std::cout << "[ics_single_communication rx_ch]:" << std::hex << c << std::endl;
////            int word = (ap_uint<32>)rx_word;
////            std::cout << "[ics_single_communication rx_word]:" << std::hex << word << std::endl;
////#endif
//        }
//        communication_memory[servo_index + RX_BUF_CYCLIC2_OFFSET] = (ap_uint<32>)rx_word + ((rx_counter + 1) << 24);
//    }//cyclic2
//}
