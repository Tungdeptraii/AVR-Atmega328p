/*
 * day5_usart.c
 *
 * Created: 23/3/2023 9:31:37 AM
 * Author: User
 */

#include <mega328p.h>
//truyen 1 ki tu du lieu 
void putchar(unsigned char data){
    while(!(UCSR0A & 0b00100000))   //cho thanh ghi du lieu EMPTY
    {
        UDR0 = data;
    }
}

//nhan 1 ki tu bang ngat RX
interrupt [USART_RXC] void usart_rx_isr(void){
    rxdata = UDR0;
}

void main(void)
{
int n;
//tinh gia tri baud rate roi dua vao thanh ghi UBRRn
n = (16*1000000)/(16*9600)-1;
UBRR0H = n>>8;
UBRR0L = n;
//cau hinh che do truyen, khung ban tin bang thanh ghi UCSRnC
UCSR0C = 0b00000110;                //truyen ko dong bo, 8 data bit, 1 stop bit, no parity
//cho phep ca 2 chieu truyen nhan, cho phep ngat nhan du lieu (RX interrupt)
UCSR0B = 0b10011000;
//cho phep ngat toan cuc
#asm("sei")

while (1)
    {
    // Please write your application code here

    }
}
