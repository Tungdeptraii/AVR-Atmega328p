/*
 * 4_b3.c
 *
 * Created: 3/11/2024 3:48:58 PM
 * Author: letung
 */

#include <mega328p.h>

void main(void)
{
    TCCR1A |= (1 << COM1A1)|(1 << WGM11);       // COM1A1 = 1 => non-inverting mode
                                                // WGM11 = 1 => fast PWM (TOP is set at ICR1)

    TCCR1B |= (1 << WGM13)|(1 << WGM12)|(1 << CS12)|(1 << CS10);
                                                // WGM13 = WGM12 = 1 => fast PWM (TOP is set at ICR1)
                                                // CS12 = CS10 => prescale = 1024

    ICR1H = 0xB7;
    ICR1L = 0x19;   // TOP = 46873

    OCR1AH = 0x3D;
    OCR1AL = 0x08;  // Compare value = 15624 (= 1/3 TOP)

    DDRB |= (1 << DDB1) | (0 << DDB5);          // Set PortB 1 as Output, PortB 5 as Input
    PORTB = (1 << PORTB5);                      // Pullup Resistor
while (1)
    {
    // Please write your application code here
}
}
