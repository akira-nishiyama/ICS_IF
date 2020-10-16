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
#include "gtest/gtest.h"
#include "ics_if_rx.h"

void input_stream(
    hls::stream<uint1_t> &ics_sig,
    unsigned char ch,
    uint10_t bit_period,
	uint1_t parity_err_flag
){
    ap_uint<8> parity = ch;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    ap_uint<11> tx_ch = (ch & 0x0ff) * 4 + ((parity + parity_err_flag) & 0x01) * 2 + 1;//add start bit, parity and stop bit.
    for(int i = 0; i < 10; ++i){
        for(int j = 0; j < bit_period; ++j){
            ics_sig.write_nb((tx_ch >> (10 - i) & 0x01));
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
class IcsIfRxTest : public testing::Test {
    protected:
    ap_uint<10> bit_period = 87;
    hls::stream<uint9_t> ics_char;
    hls::stream<uint9_t> ics_char_exp;
    hls::stream<uint1_t> ics_sig_i;
    
    IcsIfRxTest() {
    }
    
    virtual ~IcsIfRxTest() {
    }
    
    virtual void SetUp() {
    }
    
    virtual void TearDown() {
    }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST_F(IcsIfRxTest, ReceiveTest) {
    bit_period = 87;
    //initialize ics_sig_i
    //cyclic0
    //cyclic0 servo_index0
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x80,bit_period,0);ics_char_exp.write(0x80);//loopback
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//loopback
    input_stream(ics_sig_i,0x02,bit_period,0);ics_char_exp.write(0x02);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x00,bit_period,0);ics_char_exp.write(0x00);//receive
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//receive
    input_stream(ics_sig_i,0x05,bit_period,0);ics_char_exp.write(0x05);//receive
    //cyclic0 servo_index1
    input_stream(ics_sig_i,0x81,bit_period,0);ics_char_exp.write(0x81);//loopback
    input_stream(ics_sig_i,0x06,bit_period,0);ics_char_exp.write(0x06);//loopback
    input_stream(ics_sig_i,0x05,bit_period,0);ics_char_exp.write(0x05);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x01,bit_period,0);ics_char_exp.write(0x01);//receive
    input_stream(ics_sig_i,0x14,bit_period,0);ics_char_exp.write(0x14);//receive
    input_stream(ics_sig_i,0x15,bit_period,0);ics_char_exp.write(0x15);//receive
    //cyclic0 servo_index2
    input_stream(ics_sig_i,0x82,bit_period,0);ics_char_exp.write(0x82);//loopback
    input_stream(ics_sig_i,0x09,bit_period,0);ics_char_exp.write(0x09);//loopback
    input_stream(ics_sig_i,0x08,bit_period,0);ics_char_exp.write(0x08);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x02,bit_period,0);ics_char_exp.write(0x02);//receive
    input_stream(ics_sig_i,0x24,bit_period,0);ics_char_exp.write(0x24);//receive
    input_stream(ics_sig_i,0x25,bit_period,0);ics_char_exp.write(0x25);//receive
    //cyclic0 servo_index31
    input_stream(ics_sig_i,0x9f,bit_period,0);ics_char_exp.write(0x9f);//loopback
    input_stream(ics_sig_i,0x0c,bit_period,0);ics_char_exp.write(0x0c);//loopback
    input_stream(ics_sig_i,0x0b,bit_period,0);ics_char_exp.write(0x0b);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x1f,bit_period,0);ics_char_exp.write(0x1f);//receive
    input_stream(ics_sig_i,0x34,bit_period,0);ics_char_exp.write(0x34);//receive
    input_stream(ics_sig_i,0x35,bit_period,0);ics_char_exp.write(0x35);//receive
    //cyclic1
    //cyclic1 servo_index0
    input_stream(ics_sig_i,0xa0,bit_period,0);ics_char_exp.write(0xa0);//loopback
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x20,bit_period,0);ics_char_exp.write(0x20);//receive
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//receive
    input_stream(ics_sig_i,0x44,bit_period,0);ics_char_exp.write(0x44);//receive
    //cyclic1 servo_index1
    input_stream(ics_sig_i,0xa1,bit_period,0);ics_char_exp.write(0xa1);//loopback
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x21,bit_period,0);ics_char_exp.write(0x21);//receive
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//receive
    input_stream(ics_sig_i,0x45,bit_period,0);ics_char_exp.write(0x45);//receive
    //cyclic1 servo_index2
    input_stream(ics_sig_i,0xa2,bit_period,0);ics_char_exp.write(0xa2);//loopback
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x22,bit_period,0);ics_char_exp.write(0x22);//receive
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//receive
    input_stream(ics_sig_i,0x46,bit_period,0);ics_char_exp.write(0x46);//receive
    //cyclic1 servo_index31
    input_stream(ics_sig_i,0xbf,bit_period,0);ics_char_exp.write(0xbf);//loopback
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x3f,bit_period,0);ics_char_exp.write(0x3f);//receive
    input_stream(ics_sig_i,0x03,bit_period,0);ics_char_exp.write(0x03);//receive
    input_stream(ics_sig_i,0x47,bit_period,0);ics_char_exp.write(0x47);//receive
    //cyclic2
    //cyclic2 servo_index0
    input_stream(ics_sig_i,0xa0,bit_period,0);ics_char_exp.write(0xa0);//loopback
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x20,bit_period,0);ics_char_exp.write(0x20);//receive
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//receive
    input_stream(ics_sig_i,0x48,bit_period,0);ics_char_exp.write(0x48);//receive
    //cyclic2 servo_index1
    input_stream(ics_sig_i,0xa1,bit_period,0);ics_char_exp.write(0xa1);//loopback
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x21,bit_period,0);ics_char_exp.write(0x21);//receive
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//receive
    input_stream(ics_sig_i,0x49,bit_period,0);ics_char_exp.write(0x49);//receive
    //cyclic2 servo_index2
    input_stream(ics_sig_i,0xa2,bit_period,0);ics_char_exp.write(0xa2);//loopback
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x22,bit_period,0);ics_char_exp.write(0x22);//receive
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//receive
    input_stream(ics_sig_i,0x4a,bit_period,0);ics_char_exp.write(0x4a);//receive
    //cyclic2 servo_index31
    input_stream(ics_sig_i,0xbf,bit_period,0);ics_char_exp.write(0xbf);//loopback
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//loopback
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x3f,bit_period,0);ics_char_exp.write(0x3f);//receive
    input_stream(ics_sig_i,0x04,bit_period,0);ics_char_exp.write(0x04);//receive

    for(int i = 0; i < 63; ++i){
        ics_if_rx(ics_sig_i,ics_char,bit_period);
        EXPECT_EQ(ics_char_exp.read(), ics_char.read()) << "char differ at index 0x" << std::hex << i;
        //std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << std::endl;
    }
}

TEST_F(IcsIfRxTest, ReceiveParityErrTest) {
    input_idle(ics_sig_i,100);
    input_stream(ics_sig_i,0x80,bit_period,1);ics_char_exp.write(0x180);//loopback
    input_stream(ics_sig_i,0x03,bit_period,1);ics_char_exp.write(0x103);//loopback
    input_stream(ics_sig_i,0x02,bit_period,1);ics_char_exp.write(0x102);//loopback
    for(int i = 0; i < 3; ++i){
        ics_if_rx(ics_sig_i,ics_char,bit_period);
        EXPECT_EQ(ics_char_exp.read(), ics_char.read()) << "char differ at index 0x" << std::hex << i;
    }
}
/*
int old_main(void){
    std::cout << "===============receive test====================" << std::endl;
    for(int i = 0; i < 64; ++i){
        while(ics_sig_i.read() == 1);//skip until start bit. it is done by external edge detect logic to assert ap_start.
        ics_if_rx(ics_sig_i,ics_char,bit_period,0);
        std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << std::endl;
    }
    return 0;
}*/
