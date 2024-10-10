/*
 * midterm.c
 *
 * Created: 13/4/2023 9:03:52 AM
 * Author: User
 */

#include <mega328p.h>
#include <delay.h>
int n;
unsigned long ADC_val;

//truyen 1 ki tu du lieu 
void putchar(unsigned char data){
    while(!(UCSR0A & 0b00100000));   //cho thanh ghi du lieu EMPTY
    UDR0 = data;
}

void putstring(unsigned char *str){
    while(*str){
        putchar(*str);
        //if see the line feed, add carriage return
        if(*str == '\n')
            putchar('\r');
        str++;
    }
}

void put_int(unsigned int value){
    unsigned char buf[8];
    int index = 0,i,j;
    j = value;
    do{
       buf[index] = j%10 + 48;     //chuyen gia tri sang ki tu 
       j = j/10;
       index += 1;
    }
    while (j);    
    
    for(i = index; i>0; i--)
    putchar(buf[i-1]);
}

void main(void)
{
DDRD |= 0b01000000;             //Dat PORTD.6 la OUT
PORTD &= 0b10111111;
DDRD.6 = 1;
PORTD.6 = 0;

//tinh gia tri baud rate roi dua vao thanh ghi UBRRn
n = 8;   // n = 16M/(16*115200)-1 = 8
UBRR0H = n>>8;
UBRR0L = n;
//cau hinh che do truyen, khung ban tin bang thanh ghi UCSRnC
UCSR0C = 0b00000110;                //truyen ko dong bo, 8 data bit, 1 stop bit, no parity
//cho phep ca 2 chieu truyen nhan, cho phep ngat nhan du lieu (RX interrupt)
UCSR0B = 0b10011000;

//Select AVCC as reference voltage, ADC input is AD5(PC5)
ADMUX = 0b01000101;
//Enable ADC, ADC clock = 125kHz
ADCSRA = 0b10000111;
//Disable Digital Input function of AD5 pin (PC5)
DIDR0 |= (ADC5D << 1);
//Wait for the ADC circuit stable
delay_ms(10);

while (1)
    {
    // Please'/ write your application code here    
    
    //Update the ADC input 
    ADMUX = 0b01000101;
    //Disable Digital input function
    DIDR0 |= (ADC5D << 1);
    //Wait for ther ADC circuit stable
    delay_ms(10);
    //Start ADC conversion
    ADCSRA |= 0b01000000;
    //Wait for the converson complete
    while((ADCSRA & 0b00010000) == 0);  
    ADC_val = ADCW;
    //Clear the ADC conversion complete flag
    ADCSRA |= 0b00010000; 
    
    putstring("Gia tri ADC: ");
    put_int(ADC_val);
    putstring(" ");
    
    if(ADC_val >= 800){
        PORTD.6 = !PORTD.6;
        delay_ms(250);
        putstring(" System alarm. ");
    }else{
        putstring(" Alarm Over. ");
        PORTD.6 = 0;
    }
    
    }
}
