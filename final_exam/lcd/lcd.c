/*******************************************************
Project : I2C multi lcd
Version : 2.0.0
Date    : 05/13/2022
Author  : Mohammad Reza Haseli 
Comments: its about run three lcd sepreatly with i2c


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

// Declare Function
#include <mega328p.h>
#include <stdio.h>
#include <delay.h>
#include <i2c.h>
#include <LCD_i2c.h>

// Declare i2c addrees
#define LCD_1   0x27


// Declare your global variables here
int count1,count2, count3;
char data[16];

//Declare Display finction
void Display();

void main(void)
{

    // Bit-Banged I2C Bus initialization
    // I2C Port: PORTC
    // I2C SDA bit: 1
    // I2C SCL bit: 0
    // Bit Rate: 100 kHz
    // Note: I2C settings are specified in the
    // Project|Configure|C Compiler|Libraries|I2C menu.
    i2c_init();
    i2c_start();
       
    i2c_write(LCD_1);
    lcd_init();
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string("LCD_1 init :)");
    delay_ms(1000);
    lcd_clear();

while (1)
      {
        count1++;
        if (count1%2==0)
        {              
            count2++;
            if(count2%2 == 0) count3++;
        }
        Display();
        delay_ms(1000); 
                         
      }
}


void Display()
{
    //LCD1
    i2c_write(LCD_1);
    i2c_start();
    lcd_put_cur(0, 0);
    lcd_send_string("Lcd_1");
    lcd_put_cur(1, 0);
    lcd_send_string("Count = ");
    lcd_put_cur(1, 8);
    sprintf(data, "%d", count1);
    lcd_send_string(data);
                       
}