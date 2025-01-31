/*
 * 4_b1.c
 *
 * Created: 3/11/2024 3:22:00 PM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>

void main(void)
{
    CLKPR = (1<<CLKPCE); //cho phep chia tan so
    CLKPR = 0x00;       //dat he so chia la 1
    DDRD |= 0b01000000;
    PORTD  &= 0xBF;     
    
    // Configure for Timer 0 as a counter in pin T0(PD4)
    TCCR0A = 0b10000011;          
    TCCR0B = 0b00000101;           //Count on rising edge
    TCNT0 = 0x00; 
    OCR0A = 25;
while (1)
    {
   
    OCR0A +=25;
    if (OCR0A == 250){
        OCR0A = 25;
        }
    delay_ms(1000);
    }
}
