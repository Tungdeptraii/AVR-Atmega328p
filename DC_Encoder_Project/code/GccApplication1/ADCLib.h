
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>


void ADC_init(unsigned int pin){
	
	ADMUX = 0b01000000 | pin;
	ADCSRA = 0b10000111;
	DIDR0 |= 0x01 << pin;
	_delay_ms(10);
}

int read_ADC(){
//	ADMUX = 0b01000000 | pin;
//	ADCSRA = 0b10000111;
//	DIDR0 |= 0x01 << pin;
//	delay_ms(10);
	ADCSRA |= 0b01000000;
    while((ADCSRA & 0b00010000) == 0);
    ADCSRA |= 0b00010000;
    return ADCW;
	
}

