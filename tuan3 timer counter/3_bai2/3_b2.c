/*
 * les4_1.c
 *
 * Created: 3/5/2024 8:30:31 AM
 * Author: ADMIN
 */

#include <io.h>

unsigned int n;


void blinkled(unsigned int duration)
{
    n = 65536 - (int) (duration*16000000/2/1024);
    
     

}

interrupt [TIM1_OVF] void timer1_ovf_irs(void){
    PORTB.5 = !PORTB.5;
    TCNT1H = n >> 8;   //reset
    TCNT1L = n; //TCNT1L = n & 0x00FF; 
} 



void main(void)
{

    blinkled(2);
    DDRB.5 = 1;
    PORTB.5 = 0;
    TCCR1A = 0x00;    
    TCCR1B = 0b00000101;    //chia so lon nhat d la 1024 -> CS2-1-0 la 101
    //thanh ghi 16bit -> max=2^16
    //chu ki T = 1s
    //fIO = 16MHz
    //=>n=max-T*fIo/2d = 2^16-16.000.000/(2*1024)= 57724 = E17C
    TCNT1H = n >> 8;   //reset
    TCNT1L = n; //TCNT1L = n & 0x00FF;
    TIMSK1 = 0b00000001; //ngat overflow
    #asm("sei")  //(ko co ;), ngat toan cuc
while (1)
    {
    

    }
}