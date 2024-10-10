/*
 * 4_b2.c
 *
 * Created: 3/11/2024 3:45:49 PM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>

void main(void)
{

    CLKPR = (1 << CLKPCE);
    CLKPR = 0x00;
    
    DDRD |= 0b01000000;
    PORTD &= 0bBF;
    
    // Configure for Timer 0 as a counter in pin T0(PD4)
    TCCR0A = 0b10000001;          
    TCCR0B = 0b00000101;            //Clock timer = clockIO/1024         
    TCNT0 = 0x00; 
    OCR0A = 25;
    
    while (1){
    // Please write your application code here  
    delay_ms(1000);
    OCR0A += 25;
    if(OCR0A == 250){
        OCR0A = 25;
        }
    }
}
