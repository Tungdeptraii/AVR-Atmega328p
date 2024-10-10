#include <mega328p.h>
#include <delay.h>
#define scale 128
#include "usart.h"
#include "ADCLib.h"
unsigned long ADC_val;
unsigned long voltage_val;
unsigned long demi_part;
unsigned long int_part;

//Vector phuc vu ngat
interrupt [TIM1_OVF] void timer1_ovf_isr(void){   
    TCNT1H = 0x85;
    TCNT1L = 0xEE; 
}

//ADC interrupt
interrupt [ADC_INT] void ADC_ISR(void){
    putstring("Hello");
}

void main(void)   
{
    uart_init(); 
    //Select AVCC as reference voltage, ADC input is AD1(PC1)
    ADMUX = 0b01000001;
    //Enable ADC, auto conversion, ADC clock = 125kHz
    ADCSRA = 0b10001111;
    ADCSRB = 0b00000110;    //Timer 1 overflow
    //Disable Digital Input function of AD1 pin (PC1)
    DIDR0 |= 0x01 << 1;
    //Wait for the ADC circuit stable
    delay_ms(10);

    //Timer1/Counter0
    CLKPR = (1<<CLKPCE);            //Cho phep chia tan so
    CLKPR = 0x00;                   //Dat he so chia la 1
    
    TCCR1A = 0x00;                  //Dat che do Overflow(dem tu vi tri setup den vi tri cuoi)
    
    //Cau hinh nguon chia clock cho timer bang thanh ghi TCCRxB.
    TCCR1B = 0b00000100;            //Clock timer = clockIO/256
    //Dat gia tri ban dau cho thanh ghi TCNTx
    TCNT1H = 0x85;
    TCNT1L = 0xEE;
    TCNT0 = 0;
    
    //Cho phep ngat phu hop bang thanh ghi TIMSKx
    TIMSK1 = 0b00000001;            //Cho phep ngat Overflow
    //Cho phep ngat toan cuc
    #asm("sei")   
    
                     
    //Update the ADC input
    ADMUX = 0b01000001;
    //Disable Digital input function
    DIDR0 |= 0x01 << 1;
    //Wait for ther ADC circuit stable
    delay_ms(10);
    //Start ADC conversion
    ADCSRA |= 0b01000000;  
    //Wait for the converson complete
    while((ADCSRA & 0b00010000) == 0);
    
while (1)
    {
    // Please write your application code here   
    ADC_val = ADCW;
    putstring("Gia tri ADC: ");
    put_int(ADC_val);
    putstring("\n");
    voltage_val = ADC_val*500/1024;
    put_int(voltage_val);
    putstring("\n");
    
    demi_part = voltage_val % 100; 
    int_part = voltage_val / 100;
    putstring("Gia tri dien ap: ");
    put_int(int_part);
    putstring(".");
    put_int(demi_part);
    putstring(" vol \n");
       
    }
}