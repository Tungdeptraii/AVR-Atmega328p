/*
 * 2_b3.c
 *
 * Created: 2/27/2024 10:29:19 AM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>
int i = 3;
int direction = 1;
//ledStatus = 0b00000111;







void main(void)
{
DDRD = 0xFF;
PORTD = 0b00000111;


while(1){

    if (direction == 1){
    PORTD = PORTD << 1;    
    i++;
        if (i == 7) direction = -1;
        
    
  }
    else{
        PORTD= PORTD>>1;
        i--;
        if (i==3) direction = 1;
        }
        delay_ms(1000); 
     
 
 
}

}
