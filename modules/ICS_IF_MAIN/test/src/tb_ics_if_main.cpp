/************************************************************************
 * @file tb_ics_if_main.cpp
 * @brief testbench of ics_if_main.cpp
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
#include "gtest/gtest.h"

void print_result_character(
        hls::stream<uint8_t> &ics_sig
    ){
    uint8_t reg;
    std::cout << "character:" << std::endl;
    while(!ics_sig.empty()){
        ics_sig.read_nb(reg);
        std::cout << std::hex << std::setw(2) << std::setfill('0') << +reg << std::endl;
    }
}

void compare_tx_character(hls::stream<uint8_t> &tx, hls::stream<uint8_t> &tx_exp, int num){
    uint8_t val,val_exp;
    for(int i = 0; i < num; ++i){
        std::stringstream ss, ss_exp;
        tx.read_nb(val);
        tx_exp.read_nb(val_exp);
        ss     << std::setw(2) << std::setfill('0') << std::hex << (int)val;
        ss_exp << std::setw(2) << std::setfill('0') << std::hex << (int)val_exp;
        //std::cout << "obs:" << ss.str()     << std::endl;
        //std::cout << "exp:" << ss_exp.str() << std::endl;
        EXPECT_EQ(ss.str(),ss_exp.str()) << "differ tx and tx_exp index at 0x" << std::hex << i;
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
//                    std::cout << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }else{
                    data[size] = data[size] * 2;
//                    std::cout << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
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

void compare_memory_elem(ap_uint<32> elem, ap_uint<32> elem_exp, int i){
    std::stringstream ss,ss_exp;
    int val     = elem;
    int val_exp = elem_exp;
    ss     << std::setw(8) << std::setfill('0') << std::hex << val;
    ss_exp << std::setw(8) << std::setfill('0') << std::hex << val_exp;
    EXPECT_EQ(ss.str(), ss_exp.str()) << "differ memory and memory_exp index at 0x" << std::hex << (i+512) * 4;
}

//int old_main(void){
    //initialize communication memory
//    //cyclic1
//    communication_memory[32] = 0x000003a0;
//    communication_memory[33] = 0x000003a1;
//    communication_memory[34] = 0x000003a2;
//    communication_memory[63] = 0x000003bf;
//    //cyclic2
//    communication_memory[64] = 0x000004a0;
//    communication_memory[65] = 0x000004a1;
//    communication_memory[66] = 0x000004a2;
//    communication_memory[95] = 0x000004bf;


    //initialize ics_sig_i
    //cyclic0
    //cyclic0 servo_index0
//    //cyclic1
//    //cyclic1 servo_index0
//    input_stream(ics_sig_i,0xa0,bit_period);//loopback
//    input_stream(ics_sig_i,0x03,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x20,bit_period);//receive
//    input_stream(ics_sig_i,0x03,bit_period);//receive
//    input_stream(ics_sig_i,0x44,bit_period);//receive
//    //cyclic1 servo_index1
//    input_stream(ics_sig_i,0xa1,bit_period);//loopback
//    input_stream(ics_sig_i,0x03,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x21,bit_period);//receive
//    input_stream(ics_sig_i,0x03,bit_period);//receive
//    input_stream(ics_sig_i,0x45,bit_period);//receive
//    //cyclic1 servo_index2
//    input_stream(ics_sig_i,0xa2,bit_period);//loopback
//    input_stream(ics_sig_i,0x03,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x22,bit_period);//receive
//    input_stream(ics_sig_i,0x03,bit_period);//receive
//    input_stream(ics_sig_i,0x46,bit_period);//receive
//    //cyclic1 servo_index31
//    input_stream(ics_sig_i,0xbf,bit_period);//loopback
//    input_stream(ics_sig_i,0x03,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x3f,bit_period);//receive
//    input_stream(ics_sig_i,0x03,bit_period);//receive
//    input_stream(ics_sig_i,0x47,bit_period);//receive
//    //cyclic2
//    //cyclic2 servo_index0
//    input_stream(ics_sig_i,0xa0,bit_period);//loopback
//    input_stream(ics_sig_i,0x04,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x20,bit_period);//receive
//    input_stream(ics_sig_i,0x04,bit_period);//receive
//    input_stream(ics_sig_i,0x48,bit_period);//receive
//    //cyclic2 servo_index1
//    input_stream(ics_sig_i,0xa1,bit_period);//loopback
//    input_stream(ics_sig_i,0x04,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x21,bit_period);//receive
//    input_stream(ics_sig_i,0x04,bit_period);//receive
//    input_stream(ics_sig_i,0x49,bit_period);//receive
//    //cyclic2 servo_index2
//    input_stream(ics_sig_i,0xa2,bit_period);//loopback
//    input_stream(ics_sig_i,0x04,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x22,bit_period);//receive
//    input_stream(ics_sig_i,0x04,bit_period);//receive
//    input_stream(ics_sig_i,0x4a,bit_period);//receive
//    //cyclic2 servo_index31
//    input_stream(ics_sig_i,0xbf,bit_period);//loopback
//    input_stream(ics_sig_i,0x04,bit_period);//loopback
//    input_idle(ics_sig_i,100);
//    input_stream(ics_sig_i,0x3f,bit_period);//receive
//    input_stream(ics_sig_i,0x04,bit_period);//receive
//    input_stream(ics_sig_i,0x4b,bit_period);//receive




//    return 0;
//}

class IcsIfTest : public testing::Test {
    protected:
    ap_uint<10> bit_period;
    ap_uint<5> number_of_servos = 1;
    ap_uint<8> cmd_error_cnt;
    ap_uint<32> communication_memory[512] = {};
    ap_uint<32> communication_memory_exp[512] = {};
    ap_uint<8> ics_char=0;
    hls::stream<uint8_t> ics_tx_char;
    hls::stream<uint8_t> ics_tx_char_exp;
    hls::stream<uint8_t> ics_rx_char;
    hls::stream<uint1_t>  cyclic0_start;
    hls::stream<uint1_t>  cyclic1_start;
    hls::stream<uint1_t>  cyclic2_start;
    volatile ap_uint<10> dummy = 0;
    volatile ap_uint<1> ics_rx_char_rst;

    
    IcsIfTest() {
    }
    
    virtual ~IcsIfTest() {
    }
    
    virtual void SetUp() {
        cmd_error_cnt = 0;
        for(int i = 0; i < 512; ++i){
            communication_memory[i] = 0;
            communication_memory_exp[i] = 0;
        }
        //cyclic0
        for(int i = 0; i < 32; ++i){
            communication_memory[i]     = (i + ((i + 1) << 16) + ((i + 2) << 23)) | 0x80;
            communication_memory_exp[i] = (i + ((i + 1) << 16) + ((i + 2) << 23)) | 0x80;
        }
    }
    
    virtual void TearDown() {
        EXPECT_EQ(0,ics_tx_char.size());
        EXPECT_EQ(0,ics_tx_char_exp.size());
        EXPECT_EQ(0,ics_rx_char.size());
//    std::cout << "===============c_expommunication memory====================" << std::endl;
//        dump_memory(communication_memory);
//    std::cout << "===============communication memory_exp====================" << std::endl;
//        dump_memory(communication_memory_exp);
//        print_result_character(ics_tx_char);
//        std::cout << "cmd_err:" << cmd_error_cnt << std::endl;
    }
};

TEST_F(IcsIfTest, Cyclic0Single) {
    for(int i = 0; i < 32; ++i){
        ics_rx_char.write_nb(i);
        ics_rx_char.write_nb((i+2) << 1); //upper position
        ics_rx_char.write_nb(i+1); //lower position
        communication_memory_exp[256+i] = ((i+2) << 24) + ((i+1) << 16) + (1 << 8)  + i;
    }
    for(int i = 0; i < 32; ++i){
        ics_tx_char_exp.write_nb(i | 0x80);
        ics_tx_char_exp.write_nb(i+2);//upper position
        ics_tx_char_exp.write_nb(i+1);//lower position
    }

    cyclic0_start.write_nb(1);

    ap_uint<32> cyclic0_config, cyclic1_config,cyclic2_config;
    ap_uint<1> cyclic0_enable, cyclic1_enable,cyclic2_enable;
    cyclic0_config = 0x80000032;
    cyclic1_config = 0x80000032;
    cyclic2_config = 0x800003e8;
    ap_uint<16> cyclic0_interval, cyclic1_interval, cyclic2_interval;
    ics_if_main(
        0,//ap_uint<3> bit_period_config_i,
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
        ics_tx_char,//hls::stream<uint8_t> &ics_tx_char_o,
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


    EXPECT_EQ(1,cyclic0_enable);
    EXPECT_EQ(0x32,cyclic0_interval);
    EXPECT_EQ(1,cyclic1_enable);
    EXPECT_EQ(0x32,cyclic1_interval);
    EXPECT_EQ(1,cyclic2_enable);
    EXPECT_EQ(0x3e8,cyclic2_interval);
    EXPECT_EQ(868,bit_period);
    //EXPECT_EQ(memcmp(communication_memory,communication_memory_exp,512*sizeof(ap_uint<32>)),0);
    for(int i = 0; i < 512; ++i) {
        compare_memory_elem(communication_memory[i], communication_memory_exp[i],i);
    }
    compare_tx_character(ics_tx_char, ics_tx_char_exp,32*3);
    EXPECT_EQ(0,cmd_error_cnt);
}

TEST_F(IcsIfTest, Cyclic0ReceiveErr) {
    for(int i = 0; i < 32; ++i){
        ics_rx_char.write_nb(i+1);
    }
    for(int i = 0; i < 32; ++i){
        ics_tx_char_exp.write_nb(i | 0x80);
        ics_tx_char_exp.write_nb(i+2);//upper position
        ics_tx_char_exp.write_nb(i+1);//lower position
    }

    cyclic0_start.write_nb(1);

    ap_uint<32> cyclic0_config, cyclic1_config,cyclic2_config;
    ap_uint<1> cyclic0_enable, cyclic1_enable,cyclic2_enable;
    cyclic0_config = 0x80000032;
    cyclic1_config = 0x80000032;
    cyclic2_config = 0x800003e8;
    ap_uint<16> cyclic0_interval, cyclic1_interval, cyclic2_interval;
    ics_if_main(
        0,//ap_uint<3> bit_period_config_i,
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
        ics_tx_char,//hls::stream<uint8_t> &ics_tx_char_o,
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
    for(int i = 0; i < 512; ++i) {
        compare_memory_elem(communication_memory[i], communication_memory_exp[i],i);
    }
    compare_tx_character(ics_tx_char, ics_tx_char_exp,32*3);
    EXPECT_EQ(32,cmd_error_cnt);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
