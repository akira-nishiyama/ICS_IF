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
#include "gtest/gtest.h"

int get_character(
        hls::stream<uint1_t> &ics_sig,
        ap_uint<10> bit_period,
        uint8_t *result_char,
        uint1_t *result_parity
    ){
    ap_uint<1> ics_sig_result;
    unsigned int cnt = 0;
    unsigned int cnt_low = 0;
    unsigned int cnt_high = 0;
    unsigned int bit_cnt = 0;
    uint8_t data = 0;
    uint1_t parity;
        //search startbit
        unsigned int skip_cnt = 0;
        while(ics_sig.read_nb(ics_sig_result)){
            ++skip_cnt;
            if(ics_sig_result == 0){
                for(int i = 0; i < bit_period; ++i) ics_sig.read_nb(ics_sig_result);//skip start bit
                break;
            }
        }
        if(ics_sig.empty()){
            //result_char = nullptr;
            //result_parity = nullptr;
            *result_char = 0;
            *result_parity = 0;
            return 1;
        }
        bit_cnt = 0;
        while(bit_cnt <= 8){
            ics_sig.read_nb(ics_sig_result);
            if(ics_sig_result == 1) {
                ++cnt_high;
            }else{
                ++cnt_low;
            }
            ++cnt;
            if(cnt >= bit_period && bit_cnt <= 7){
                if(cnt_high > cnt_low){
                    data = data * 2 + 1;
                    //std::cout << std::setw(2) << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }else{
                    data = data * 2;
                    //std::cout << std:: setw(2) << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }
                cnt_high = 0;
                cnt_low = 0;
                cnt = 0;
                ++bit_cnt;
            } else if( cnt >= bit_period ){
                if(cnt_high > cnt_low){
                    parity = 1;
                } else{
                    parity = 0;
                }
                //std::cout << std:: setw(2) << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                break;
            }
        }
//        std::cout << "byte:" << std::setw(2) << std::setfill('0') << std::hex << data[size] << std::endl;
        //bit_cnt = 0;
        //++size;
        //data[size] = 0;
    *result_char = data;
    *result_parity = parity;
//    std::cout << std::setw(2) << std::setfill('0') << "data:" << std::hex << (int)data << " , parity:" << parity << std::endl;
    return 0;
}

void check_direction(
    hls::stream<uint1_t> &ics_sig_dir,
    ap_uint<10> bit_period
){
    ap_uint<1> ics_sig_dir_result;
    for(int i = 0; i < bit_period * 2; ++i){
        ics_sig_dir.read_nb(ics_sig_dir_result);
        EXPECT_EQ(0,ics_sig_dir_result);
    }
    for(int i = 0; i < bit_period*10; ++i){
        ics_sig_dir.read_nb(ics_sig_dir_result);
        EXPECT_EQ(1,ics_sig_dir_result);
    }
    ics_sig_dir.read_nb(ics_sig_dir_result);
    EXPECT_EQ(0,ics_sig_dir_result);
}

void print_result_character(
        hls::stream<uint1_t> &ics_sig,
        ap_uint<10> bit_period
    ){
    ap_uint<1> ics_sig_result;
    unsigned int cnt = 0;
    unsigned int cnt_low = 0;
    unsigned int cnt_high = 0;
    unsigned char bit_cnt = 0;
    unsigned int data[64] = {};
    unsigned int parity[64] = {};
    unsigned char size = 0;
    while(!ics_sig.empty()){
        //search startbit
        unsigned int skip_cnt = 0;
        while(ics_sig.read_nb(ics_sig_result)){
            ++skip_cnt;
            if(ics_sig_result == 0){
//                std::cout << "search start bit(skip num):" << skip_cnt << std::endl;
                for(int i = 0; i < bit_period; ++i) ics_sig.read_nb(ics_sig_result);//skip start bit
                break;
            }
        }
        if(ics_sig.empty()) break;
        while(bit_cnt <= 8){
            ics_sig.read_nb(ics_sig_result);
            if(ics_sig_result == 1) {
                ++cnt_high;
            }else{
                ++cnt_low;
            }
            ++cnt;
            if(cnt >= bit_period && bit_cnt <= 7){
                if(cnt_high > cnt_low){
                    data[size] = data[size] * 2 + 1;
                    std::cout << std::setw(2) << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }else{
                    data[size] = data[size] * 2;
                    std::cout << std:: setw(2) << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }
                cnt_high = 0;
                cnt_low = 0;
                cnt = 0;
                ++bit_cnt;
            } else if( cnt >= bit_period ){
                if(cnt_high > cnt_low){
                    parity[size] = 1;
                } else{
                    parity[size] = 0;
                }
                std::cout << std:: setw(2) << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                break;
            }
        }
//        std::cout << "byte:" << std::setw(2) << std::setfill('0') << std::hex << data[size] << std::endl;
        bit_cnt = 0;
        ++size;
        data[size] = 0;
    }
    std::cout << "character:";
    for(int i = 0; i < size; ++i){
        std::cout << std::setw(2) << std::setfill('0')<< "data:" << std::hex << data[i] << " , parity:" << parity[i];
    }
    std::cout << std::endl;
}

void print_result_direction(
        hls::stream<uint1_t> &ics_sig,
        ap_uint<10> bit_period
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
                    //std::cout << "high;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
                }else{
                    data[size] = data[size] * 2;
                    //std::cout << "low;cnt_high=" << std::dec << cnt_high << ";cnt_low=" << cnt_low << std::endl;
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

class IcsIfTxTest : public testing::Test {
    protected:
    ap_uint<10>          bit_period;
    hls::stream<uint8_t> ics_char;
    hls::stream<uint8_t> ics_char_exp;
    hls::stream<uint1_t> ics_parity_exp;
    hls::stream<uint1_t> direction_exp;
    hls::stream<uint1_t> ics_sig_o;
    hls::stream<uint1_t> direction;
    uint8_t              ch;
    uint1_t              p;
    
    IcsIfTxTest() {
    }
    
    virtual ~IcsIfTxTest() {
    }
    
    virtual void SetUp() {
        //initialize ics_sig_i
        //cyclic0
        //cyclic0 servo_index0ics_char
        ics_char.write(0x80);ics_char_exp.write(0x80);ics_parity_exp.write(1);
        ics_char.write(0x03);ics_char_exp.write(0x03);ics_parity_exp.write(0);
        ics_char.write(0x02);ics_char_exp.write(0x02);ics_parity_exp.write(1);
        //cyclic0 servo_index1
        ics_char.write(0x81);ics_char_exp.write(0x81);ics_parity_exp.write(0);
        ics_char.write(0x06);ics_char_exp.write(0x06);ics_parity_exp.write(0);
        ics_char.write(0x05);ics_char_exp.write(0x05);ics_parity_exp.write(0);
        //cyclic0 servo_index2
        ics_char.write(0x82);ics_char_exp.write(0x82);ics_parity_exp.write(0);
        ics_char.write(0x09);ics_char_exp.write(0x09);ics_parity_exp.write(0);
        ics_char.write(0x08);ics_char_exp.write(0x08);ics_parity_exp.write(1);
        //cyclic0 servo_index31
        ics_char.write(0x9f);ics_char_exp.write(0x9f);ics_parity_exp.write(0);
        ics_char.write(0x0c);ics_char_exp.write(0x0c);ics_parity_exp.write(0);
        ics_char.write(0x0b);ics_char_exp.write(0x0b);ics_parity_exp.write(1);
        //cyclic1
        //cyclic1 servo_index0
        ics_char.write(0xa0);ics_char_exp.write(0xa0);ics_parity_exp.write(0);
        ics_char.write(0x03);ics_char_exp.write(0x03);ics_parity_exp.write(0);
        //cyclic1 servo_index1
        ics_char.write(0xa1);ics_char_exp.write(0xa1);ics_parity_exp.write(1);
        ics_char.write(0x03);ics_char_exp.write(0x03);ics_parity_exp.write(0);
        //cyclic1 servo_index2
        ics_char.write(0xa2);ics_char_exp.write(0xa2);ics_parity_exp.write(1);
        ics_char.write(0x03);ics_char_exp.write(0x03);ics_parity_exp.write(0);
        //cyclic1 servo_index31
        ics_char.write(0xbf);ics_char_exp.write(0xbf);ics_parity_exp.write(1);
        ics_char.write(0x03);ics_char_exp.write(0x03);ics_parity_exp.write(0);
        //cyclic2
        //cyclic2 servo_index0
        ics_char.write(0xa0);ics_char_exp.write(0xa0);ics_parity_exp.write(0);
        ics_char.write(0x04);ics_char_exp.write(0x04);ics_parity_exp.write(1);
        //cyclic2 servo_index1
        ics_char.write(0xa1);ics_char_exp.write(0xa1);ics_parity_exp.write(1);
        ics_char.write(0x04);ics_char_exp.write(0x04);ics_parity_exp.write(1);
        //cyclic2 servo_index2
        ics_char.write(0xa2);ics_char_exp.write(0xa2);ics_parity_exp.write(1);
        ics_char.write(0x04);ics_char_exp.write(0x04);ics_parity_exp.write(1);
        //cyclic2 servo_index31
        ics_char.write(0xbf);ics_char_exp.write(0xbf);ics_parity_exp.write(1);
        ics_char.write(0x04);ics_char_exp.write(0x04);ics_parity_exp.write(1);
    }
    
    virtual void TearDown() {
        EXPECT_EQ(0,ics_char_exp.size());
        EXPECT_EQ(0,ics_parity_exp.size());
    }
};

TEST_F(IcsIfTxTest, TransferBaud1152000) {
    bit_period = 87;
    for(int i = 0; i < 28; ++i){
        uint8_t ch_exp;
        uint1_t p_exp;
        ics_if_tx(ics_sig_o, direction,ics_char,bit_period);
        EXPECT_EQ(0,get_character(ics_sig_o,bit_period, &ch, &p));
        ics_char_exp.read_nb(ch_exp);
        ics_parity_exp.read_nb(p_exp);
        EXPECT_EQ(ch_exp, ch) << "differ at index "  << i;
        EXPECT_EQ(p_exp, p) << "differ at index " << i;
        check_direction(direction,bit_period);
        //print_result_character(ics_sig_o,bit_period);
        //print_result_direction(direction,bit_period);
//        std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << ",dir:" << (int)direction.read() << std::endl;
    }
    
    EXPECT_EQ(1, get_character(ics_sig_o,bit_period, &ch, &p));//no remained data check.
}

TEST_F(IcsIfTxTest, TransferBaud115200) {
    bit_period = 868;
    for(int i = 0; i < 28; ++i){
        uint8_t ch_exp;
        uint1_t p_exp;
        ics_if_tx(ics_sig_o, direction,ics_char,bit_period);
        EXPECT_EQ(0,get_character(ics_sig_o,bit_period, &ch, &p));
        ics_char_exp.read_nb(ch_exp);
        ics_parity_exp.read_nb(p_exp);
        EXPECT_EQ(ch_exp, ch) << "differ at index "  << i;
        EXPECT_EQ(p_exp, p) << "differ at index " << i;
        check_direction(direction,bit_period);
        //print_result_character(ics_sig_o,bit_period);
        //print_result_direction(direction,bit_period);
//        std::cout << "char:" << std::setw(2) << std::setfill('0') << std::hex << (int)ics_char.read() << ",dir:" << (int)direction.read() << std::endl;
    }
    
    EXPECT_EQ(1, get_character(ics_sig_o,bit_period, &ch, &p));//no remained data check.
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
