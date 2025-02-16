/*
 * bt2.c
 *
 * Created: 15/3/2023 10:00:27 PM
 * Author: Tuan Anh
 */

#include <mega328p.h>

//Vector phuc vu ngat
interrupt [TIM1_OVF] void timer1_ovf_isr(void){
    dieuKhien(1);
    PORTB.5 = !PORTB.5;
}

void dieuKhien(unsigned char f){
    int value, n;
    value = 16*1000000/(f*2*1024)  
    n = 65536 - value;
    
    TCNT1H = n;
    TCNT1L = n >> 8;
}

void main(void){
    
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0x00;                   //Dat he so chia la 1
    
    DDRB != 0b00100000;             //Dat PORTB.5 la OUT
    PORB &= 0b11011111;
    
    TCCR1A = 0x00;                  //Dat che do Overflow(dem tu vi tri setup den vi tri cuoi)
    
    //Cau hinh nguon chia clock cho timer bang thanh ghi TCCRxB.
    TCCR1B = 0b00000101;            //Clock timer = clockIO/1024
    
    //Cho phep ngat phu hop bang thanh ghi TIMSKx
    TIMSK1 = 0b00000001;            //Cho phep ngat Overflow
    #asm("sei")                     //Lenh asm cho phep ngat toan cuc
    
    while (1){
    // Please write your application code here
    }
}