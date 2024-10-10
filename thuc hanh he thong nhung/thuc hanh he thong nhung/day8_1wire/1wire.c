/*
 * 1wire.c
 *
 * Created: 10/5/2023 11:38:52 PM
 * Author: Truong Tuan Anh
 */
#include <mega328p.h>
#include "1wire_lib.h"
#include "1wire_lib.c"
#include "usart.h"

float temperature;
int i;
// Mang luu dia chi cua rom
unsigned char roms[8];

void main(void)
{
uart_init();
while (1)
    {
    // Please write your application code here
    
    /*temperature = ds18b20_gettemp(); 
    temp_value = (int)(temperature*1000);
    temp_int = temp_value/1000;
    temp_dec = temp_value%1000;

    put_str("Temperature: ");
    put_int(temperature);
    put_str(".");
    put_int(temp_dec);  
    put_str("C\n");  
    delay_ms(1000);
    */
    
    ds18b20_setresolution(12); // Set resolution to(9,10,11 or 12) bits
    temperature = ds18b20_gettemp();
    // In gia tri nhiet do
    put_str("Temperature: ");
    put_float(temperature, 3);// gia tra tri chinh xac den 3 dau phay
    put_str("°C\n");
    delay_ms(1000);
    
    // Tim ROM code   
    search_rom(roms);  
    
    // In ROM code
    put_str("ROM Address: ");
    for( i = 0; i < 8; i++){
        put_str(" ");
        put_int(roms[i]);
    }
    put_str("\n");
    delay_ms(1000); 
    
    }
}
