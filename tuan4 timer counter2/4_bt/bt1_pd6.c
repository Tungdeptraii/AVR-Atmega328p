/*
 * bt1_pd6.c
 *
 * Created: 3/12/2024 9:47:14 AM
 * Author: letung
 */

#include <mega328p.h>



void main(void)
{

    CLKPR = (1 << CLKPCE); //chia tan so
    CLKPR = 0b00000101; //he so chia la 32
    
    DDRD |= 0b01000000;
    PORTD &= 0xBF;
    
    TCCR0A = 0b01000010; 
    TCCR0B = 0b00000101; //pre = 1024   
    OCR0A = 244;
    
    #asm("sei")
    



while (1)
    {
    // Please write your application code here

    }
}
