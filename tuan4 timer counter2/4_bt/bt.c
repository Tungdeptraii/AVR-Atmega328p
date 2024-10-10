/*
 * bt.c
 *
 * Created: 3/12/2024 8:41:31 AM
 * Author: letung
 */
#include <mega328p.h>

interrupt [TIM1_OVF] void timer1_ovf_irs(void){
    PORTB.5 = !PORTB.5;
    PORTD |= 0b00010000;
    TCNT1H = 0xD9;   //reset
    TCNT1L = 0xD9;
    PORTD = (TCNT0 | 0b00010000);
} 

void main(void)
{
    DDRB  |= 0b00100000;
    PORTB &= 0b11011111;
    DDRD |= 0b11101111;
    PORTD &= 0b00010000;
    
    TCCR0B = 0b00000111;
    
    TCCR1A = 0x00;    
    TCCR1B = 0b00000101;    //pre = 1024
    //thanh ghi 16bit -> max=2^16
    //chu ki T = 1s --> n = D9D9;

    TCNT1H = 0xD9;
    TCNT1L = 0xD9;
    TIMSK1 = 0x01;
    
    
    
    TCCR0A = 0x00;
    TCCR0B |= 0b00000111;
    TCNT0 = 0x00;          
    #asm("sei")  
while (1)
    {
    if (TCNT0 ==10){
         TCNT0 = 0x00;
        }

    }
}