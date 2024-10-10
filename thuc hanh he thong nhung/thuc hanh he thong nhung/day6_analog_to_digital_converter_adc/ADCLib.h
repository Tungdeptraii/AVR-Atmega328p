#include <mega328p.h>
#include <delay.h>

#define ADC_VREF_TYPE ((0<<REFS1)|(1<<REFS0)|(0<<ADLAR))

void adc_init(unsigned char input) {
    //Select AVCC as reference voltage, ADC input is AD0(PC0)
    ADMUX = ADC_VREF_TYPE | input;
    //Enable ADC, single conversion mode, ADC clock = 125kHz
    ADCSRA = 0b10000111;
    //Disable Digital Input function of AD0 pin (PC0)
    DIDR0 |= 0x01 << input;
    //Wait for the ADC circuit stable
    delay_ms(10);
}  
  
 
unsigned int ADC_read (unsigned char input){
    //Update the ADC input
    ADMUX = ADC_VREF_TYPE | input;
    //Disable Digital input function
    DIDR0 |= 0x01 << input;
    //Wait for ther ADC circuit stable
    delay_ms(10);
    //Start ADC conversion
    ADCSRA |= 0b01000000;
    //Wait for the converson complete
    while((ADCSRA & 0b00010000) == 0);
    //Clear the ADC conversion complete flag
    ADCSRA |= 0b00010000;
    return ADCW;
}