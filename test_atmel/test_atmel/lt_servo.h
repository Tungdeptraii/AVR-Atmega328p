/*
 * lt_servo.h
 *
 * Created: 5/30/2024 2:45:46 PM
 *  Author: letung
 */ 


#ifndef LT_SERVO_H_
#define LT_SERVO_H_

#include <avr/io.h>

unsigned int angle = 0;

void servo_init(void)
{
	DDRB |= (1 << DDB1);
	TCCR1A = 0; TCCR1B = 0;
	TCCR1A |= (1 << WGM11); TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS11);
	OCR1AH = 0x03;
	OCR1AL = 0xE8;

	ICR1H = 0x9C;
	ICR1L = 0x40;
	sei();
}

void servo_set_angle(unsigned int x)
{
	x = 1000 + x * 21;
	if (angle != x) {
		OCR1AH = x >> 8;
		OCR1AL = x & 0x000000FF;
		angle = x;
	}
	else {
		return;
	}
}




#endif /* LT_SERVO_H_ */