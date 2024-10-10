/*
 * GccApplication1.c
 *
 * Created: 5/23/2024 3:04:35 PM
 * Author : letung
 */ 
#define F_CPU 16000000UL
#define deltaT 0.04
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "uart.h"
#include "ADCLib.h"
#include "hd44780pcf8574.h"


char addr = PCF8574_ADDRESS;  //0x27
float ADC_value = 0;
float setpoint = 0;
float pos = 0;

int b;
char str_sp[5];
char str_pv[5]; 
char str_angle[5];

long prevT = 0;
float eprev = 0;
float eintegral = 0;

bool Saturation = false;

// PID parameters
float kp = 6.75;
float ki = 12.14;
//float ki = 0;
float kd = 0.22;

ISR(INT0_vect){
	b = PIND & (1 << PD3); 
	if (b) pos++;
	else pos--; 	
}
void setMotor(int dir, int pwmVal) {
	OCR0B = pwmVal;
	if (dir == 1) {
		PORTD |= (1 << PD7);
		PORTD &= ~(1 << PD6);
		} else if (dir == -1) {
		PORTD &= ~(1 << PD7);
		PORTD |= (1 << PD6);
		} else {
		PORTD &= ~(1 << PD7);
		PORTD &= ~(1 << PD6);
	}

}
int main(void)
{
	HD44780_PCF8574_Init(addr);
	HD44780_PCF8574_DisplayClear(addr);
	HD44780_PCF8574_DisplayOn(addr);
	HD44780_PCF8574_DrawString(addr, "Embedded System");
	_delay_ms(3000);
	HD44780_PCF8574_DisplayClear(addr);
	HD44780_PCF8574_PositionXY(addr, 0, 0);
	HD44780_PCF8574_DrawString(addr,"SP:");
	HD44780_PCF8574_PositionXY(addr, 0, 1);
	HD44780_PCF8574_DrawString(addr,"PV:");
	//_delay_ms(100);
    uart_init(9600);
    ADC_init(0);
	
	DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0B1); // Fast PWM, non-inverting mode
	TCCR0B |= (1 << CS00) | (1 << CS01); // Prescaler 64
	
	EICRA=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
	EIMSK=(0<<INT1) | (1<<INT0);
	EIFR=(0<<INTF1) | (1<<INTF0);
	sei();

	
  
    while (1) 
    {
		
		read_ADC();
		ADC_value = ADCW;
		ADC_value = 10*round(ADC_value*36/1023);
		setpoint = round((11 * 99.3 * ADC_value)/360);
		int e = setpoint - pos;
		float dedt = (e - eprev) / deltaT;
		if (!Saturation) {
			eintegral += (e + eprev) * deltaT / 2;
		}
		// PID calculation
		float u = kp * e + ki * eintegral + kd * dedt;

		float pwr = fabs(u);
		if (pwr > 255) {
			pwr = 255;
			Saturation = true;
			} else {
			Saturation = false;
		}

		// Motor direction
		int dir;
		if (u < 0) {
			dir = -1;
			} else {
			dir = 1;
		}

		// Set motor
		setMotor(dir, pwr);

		// Store previous error
		eprev = e;
		
		sprintf(str_angle, "%4.0f",ADC_value);
		sprintf(str_sp, "%4.0f",setpoint);
		sprintf(str_pv, "%4.0f",pos);
		HD44780_PCF8574_PositionXY(addr, 4, 0);
		HD44780_PCF8574_DrawString(addr,str_angle);
		HD44780_PCF8574_PositionXY(addr, 10, 0);
		HD44780_PCF8574_DrawString(addr,str_sp);
		HD44780_PCF8574_PositionXY(addr, 10, 1);
		HD44780_PCF8574_DrawString(addr,str_pv);

		//uart_put_int(0);
		//uart_put_int(1200);
		uart_putstring("Setpoint:");
		uart_putstring(str_sp);
		uart_putstring("   ");
		uart_putstring(" PV:");
		uart_putstring(str_pv);
		uart_putstring("\n");
    }
}

