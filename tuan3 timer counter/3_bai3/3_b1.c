/*
 * 3_b1.c
 *
 * Created: 3/5/2024 11:44:22 AM
 * Author: letung
 */

#include <io.h>


unsigned int n = 0;

interrupt [TIM1_OVF] void timer1_ovf_irs(void){
    n++;
    if(n==2) PORTB.5 =0;
    if (n==6) {
     PORTB.5 = 1;
     n=0;
    }
    
    TCNT1H = 0xE1;   //reset
    TCNT1L = 0x7C;
} 

void main(void)
{
    DDRB.5 = 1;
    PORTB.5 = 0;
    TCCR1A = 0x00;    
    TCCR1B = 0b00000101;    //chia so lon nhat d la 1024 -> CS2-1-0 la 101
    TCNT1H = 0xE1;
    TCNT1L = 0x7C;
    TIMSK1 = 0b00000001; //ngat overflow
    #asm("sei")  // ngat toan cuc
while (1)
    {
    

    }
}