#include <mega328p.h>

void main(void){
    
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0x00;                   //Dat he so chia la 16

    DDRB = 0b00000010;              //Dat PB1 la OUT
    PORTB = 0b11111101;
                                                                          
    // Configure for Timer 1 as a counter in pin (PB1)
    TCCR1A = 0b10000010;            //fast PWM , TOP = ICR1, update at BOTTOM 
    TCCR1B = 0b00011101;            //Clock timer = clockIO/1024     
    //46874
    ICR1H = 0xB7;
    ICR1L = 0x1A;  
    //15625
    OCR1AL = 0x3D;
    OCR1AH = 0x07; 
        
    while (1){
    // Please write your application code here  
    
    }
}
