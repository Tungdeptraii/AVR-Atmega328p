/*
 * 2_b3.c
 *
 * Created: 2/27/2024 10:29:19 AM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>


void blinkLedD(unsigned char pin, unsigned int duration){
    PORTD = PORTD | (0x01<<pin); //0b00000001 << pin
    // 0 | 00010000 = 00010000
    delay_ms(duration);
    PORTD = PORTD & (~(0x01<<pin)); //0b00000001 << 4 = 0b00010000
    // ~0b00010000 = 0b11101111
	// 0b00010000 & 0b11101111 = 0b00000000  
    delay_ms(duration);
    
    



}

void main(void)
{
DDRD = 0xFF; // 0b11111111
PORTD = 0;






while (1)
    {
    // Please write your application code here
    
        blinkLedD(4,100);
    }
}
