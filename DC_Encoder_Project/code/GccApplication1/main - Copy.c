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
#include "uart.h"
#include "ADCLib.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

float ADC_value = 0;
float setpoint = 0;

int b, pos;
char out_str[30];

long prevT = 0;
float eprev = 0;
float eintegral = 0;

bool Saturation = false;

// PID parameters
float kp = 9.18;
float ki = 86.74;
float kd = 0.23;

ISR(INT0_vect){
	b = PIND & (1 << 3);
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
		ADC_value = round(ADC_value*360/1023);
		setpoint = (11 * 99.3 * ADC_value) / 360;
		
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
		int dir = (u < 0) ? -1 : 1;

		// Set motor
		setMotor(dir, pwr);

		// Store previous error
		eprev = e;
		
		sprintf(out_str, "ADC: %f pos: %d\n", setpoint, pos);
		
		uart_putstring(out_str);
		uart_putstring("\n");
    }
}

