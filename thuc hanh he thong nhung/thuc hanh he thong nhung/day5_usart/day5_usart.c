/*
 * day5_usart.c
 *
 * Created: 23/3/2023 9:31:37 AM
 * Author: User
 */

#include <mega328p.h>
#define BAUDRATE 9600
#define fOSC 16000000
#include <delay.h>
int n;

//truyen 1 ki tu du lieu 
void putchar(unsigned char data){
    while(!(UCSR0A & 0b00100000));   //cho thanh ghi du lieu EMPTY
    UDR0 = data;
}

//nhan 1 ki tu bang ngat RX
//interrupt [USART_RXC] void usart_rx_isr(void){
//    unsigned char rxdata;
//    rxdata = UDR0; 
//    putchar(rxdata);
//}

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
//tinh gia tri baud rate roi dua vao thanh ghi UBRRn
n = fOSC/(16*BAUDRATE)-1;
UBRR0H = n>>8;
UBRR0L = n;
//cau hinh che do truyen, khung ban tin bang thanh ghi UCSRnC
UCSR0C = 0b00000110;                //truyen ko dong bo, 8 data bit, 1 stop bit, no parity
//cho phep ca 2 chieu truyen nhan, cho phep ngat nhan du lieu (RX interrupt)
UCSR0B = 0b10011000;

putchar('a');
putstring("chao mung ngay anh Banh ra tu");
put_int(123);

//cho phep ngat toan cuc
#asm("sei")

while (1)
    {
    // Please'/ write your application code here   
      
    }
}
