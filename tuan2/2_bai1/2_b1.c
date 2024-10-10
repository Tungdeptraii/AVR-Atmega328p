/*
 * 2_b1.c
 *
 * Created: 2/27/2024 10:24:42 AM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>

void main(void)
{
DDRB.5 = 1; //set output
PORTB.5 = 0;





while (1)
    {
    PORTB.5 = !PORTB.5;
    delay_ms(1000);

    }
}
