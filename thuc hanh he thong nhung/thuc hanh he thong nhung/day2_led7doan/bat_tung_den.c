/*
 * bat_tung_den.c
 *
 * Created: 2/3/2023 9:32:14 AM
 * Author: User
 */

#include <mega328p.h>
#include <delay.h>

void main(void)
{
    DDRD = 0b11111111;  
    PORTD = 0b00000000;  
    DDRB.0 = 0;         //Dat PB0 lam INPUT
    PORTB.0 = 1;        //Kich hoat dien tro keo len cho PB0
      
while (1)
    {
    // Please write your application code here    
    
    // den sang lan luot
//    PORTD = (PORTD << 1)|(1);
//    delay_ms(1000);
//    if(PORTD == 0xFF){ 
//        //PORTD = 0x00;  
//        while(PORTD != 0x00){
//            PORTD = (PORTD >> 1);
//            delay_ms(1000);  
//        }
//         
//        }

        // dung chan INPUT de dieu khien den bat/tat
        if(PINB.0 == 1){
            PORTD = 0b10101010;
        }else
            {
                PORTD = 0x00;
            }
    }    
}
