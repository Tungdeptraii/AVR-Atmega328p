/*
 * code.c
 *
 * Created: 5/21/2024 10:09:58 PM
 * Author: letung
 */

#include <mega328p.h>
#include <uart.h>
#include <delay.h>
#include <ADCLib.h>
#include <stdio.h>

#define ENC_A 2
#define ENC_B 3
//#define PWM 5
//#define IN3 7
//#define IN4 6
int b, pos = 0;
char test[32];
int sp = 0;

float ADC_value;

interrupt [EXT_INT0] void ext_int0_isr(void){
    if (b) pos++;
    else pos--;      
    
} 

void setMotor(int dir, int pwmVal) {
    OCR0B = pwmVal;
    if (dir == 1) {
        PORTD |= (1 << 7);
        PORTD &= ~(1 << 6);
    } else if (dir == -1) {
        PORTD &= ~(1 << 7);
        PORTD |= (1 << 6);
    } else {
        PORTD &= ~(1 << 7);
        PORTD &= ~(1 << 6);
    }

}
void main(void)
{
     
    DDRD |= (1 << 5) | (1 << 7) | (1 << 6);
    TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0B1); // Fast PWM, non-inverting mode
    TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler 64

    // Initialize Timer1
    TCCR1B |= (1 << CS11);

    uart_init(9600);
    ADC_init(0); 
    EICRA=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
    EIMSK=(0<<INT1) | (1<<INT0);
    EIFR=(0<<INTF1) | (1<<INTF0);
    #asm("sei")
while (1)
    {   
    setMotor(-1, 255);
    read_ADC();
    ADC_value = ADCW; 
    ADC_value = (ADC_value*360)/1023;
   
    //sp = (11 * 99.3 * ADC_value) / 360;     
    uart_put_int(sp);
    uart_putstring(" "); 
    //uart_put_int(sp);
    
    sprintf(test, "ADC: %f\n", ADC_value);
    uart_putstring(test);    
    uart_putstring("\n"); 
    delay_ms(1000);
     
      
    }
}

