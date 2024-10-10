#include <mega328p.h>

//Vector phuc vu ngat
//interrupt [TIM1_OVF] void timer1_ovf_isr(void){
//    PORTB.5 = !PORTB.5;
//}

void main(void){
    
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0x00;                   //Dat he so chia la 16

    DDRD = 0b01000000;
    PORTD = 0x00;
    
    //Cho phep ngat phu hop bang thanh ghi TIMSKx
    TIMSK1 = 0b00000001;            //Cho phep ngat Overflow                                                                        
    // Configure for Timer 0 as a counter in pin T0(PD4)
    TCCR0A = 0b10000011;          
    TCCR0B = 0b00000101;           //Count on rising edge
    TCNT0 = 0x00; 
    OCR0A = 25;
    
    while (1){
    // Please write your application code here

    }
}
