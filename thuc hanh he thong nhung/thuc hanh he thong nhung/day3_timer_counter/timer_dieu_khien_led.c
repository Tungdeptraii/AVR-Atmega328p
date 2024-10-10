#include <mega328p.h>

//Vector phuc vu ngat
interrupt [TIM0_COMPA] void timer0_compa_isr(void){
    PORTD.5 = !PORTD.5;
    PORTD.4 = 1;
}

void main(void){
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0b00000101;             //Dat he so chia la 16
    
    DDRD |= 0b01110000;             //Dat chan PD6 la OUT
    PORTD = 0x00;
    
    TCCR0A = 0b01000010;               
    //Cau hinh nguon chia clock cho timer bang thanh ghi TCCRxB.
    TCCR0B = 0b00000101;            //Clock timer = clockIO/1024 
    OCR0A = 244;
    //Dat gia tri ban dau cho thanh ghi TCNTx
//    TCNT1H = 0xD9;
//    TCNT1L = 0xD9;
//    
//    //Cho phep ngat phu hop bang thanh ghi TIMSKx
    TIMSK0 = 0b00000010;            //Cho phep ngat Overflow                                                                        
//    // Configure for Timer 0 as a counter in pin T0(PD4)
//    TCCR0A = 0x00;          
//    TCCR0B |= 0b00000111;           //Count on rising edge
//    TCNT0 = 0x00;
//                    
    #asm("sei")                     //Lenh asm cho phep ngat toan cuc 
    
    while (1){
    // Please write your application code here

    }
}