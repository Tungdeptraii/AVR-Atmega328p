/*
 * bt1.c
 *
 * Created: 15/3/2023 10:00:27 PM
 * Author: Tuan Anh
 */

#include <mega328p.h>

//Vector phuc vu ngat
interrupt [TIM1_OVF] void timer1_ovf_isr(void){
    PORTB.5 = !PORTB.5;
    TCNT1H = 0x85;
    TCNT1L = 0xEE; 
}

void main(void){
    
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0x00;                   //Dat he so chia la 1
    
    DDRB != 0b00100000;             //Dat PORTB.5 la OUT
    PORTB &= 0b11011111;
    
    TCCR1A = 0x00;                  //Dat che do Overflow(dem tu vi tri setup den vi tri cuoi)
    
    //Cau hinh nguon chia clock cho timer bang thanh ghi TCCRxB.
    TCCR1B = 0b00000100;            //Clock timer = clockIO/256
    //Dat gia tri ban dau cho thanh ghi TCNTx
    TCNT1H = 0x85;
    TCNT1L = 0xEE;
    
    //Cho phep ngat phu hop bang thanh ghi TIMSKx
    TIMSK1 = 0b00000001;            //Cho phep ngat Overflow
    #asm("sei")                     //Lenh asm cho phep ngat toan cuc
    
    while (1){
    // Please write your application code here
    

    }
}