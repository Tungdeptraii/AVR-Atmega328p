/*
 * les4_1.c
 *
 * Created: 3/5/2024 8:30:31 AM
 * Author: ADMIN
 */

#include <io.h>

interrupt [TIM1_OVF] void timer1_ovf_irs(void){
    PORTB.5 = !PORTB.5;
    TCNT1H = 0xE1;   //reset
    TCNT1L = 0x7C;
} 

void main(void)
{
    DDRB.5 = 1;
    PORTB.5 = 0;
    TCCR1A = 0x00;    
    TCCR1B = 0b00000101;    //chia so lon nhat d la 1024 -> CS2-1-0 la 101
    //thanh ghi 16bit -> max=2^16
    //chu ki T = 1s
    //fIO = 16MHz
    //=>n=max-T*fIo/2d = 2^16-16.000.000/(2*1024)= 57724 = E17C
    TCNT1H = 0xE1;
    TCNT1L = 0x7C;
    TIMSK1 = 0b00000001; //ngat overflow
    #asm("sei")  //(ko co ;), ngat toan cuc
while (1)
    {
    // Please write your application code here

    }
}