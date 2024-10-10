/*
 * 2_b3.c
 *
 * Created: 2/27/2024 10:29:19 AM
 * Author: letung
 */

#include <mega328p.h>
#include <delay.h>
int i = 0;







void main(void)
{
DDRD = 0xFF; //0b11111111
PORTD &= 0x00; // 0b00000000
DDRB = 0xFE; // 0b11111110

while(1){
 if (PINB.0 ==0){
  PORTD = 0;
 }
 else{
  PORTD = 0x01; // 0b00000001
  PORTD = PORTD << i; 
  // 0b00000010
  // 0b00000100
  // 0b00001000
  // 0b00010000
  // 0b00100000
  // 0b01000000
  // 0b10000000
  
  
  i++;
  if (i>7) i=0;
  delay_ms(1000);
 
 
 
 }
}

}
