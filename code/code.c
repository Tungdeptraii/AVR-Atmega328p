/*
 * TWI.c
 *
 * Created: 5/16/2023 3:48:00 PM
 * Author: Tuan Do 
 * This code implements a general code for TWI in all mode: 
 Master transmit, Master receive, Slave transmint and Slave receive
 * Note: Choose a unique address for each device before burning code to the chip by change 
 the value of own_address 
              #define own_address 2
 
 */

#include <mega328p.h>
#include <delay.h>
#include "uart.h"
#include "TWI_Lib.h"

////////////////////////////////////////////////////////////
#define own_address 1
#define slave_address 2


////////////////////////////////////////////////////////////
unsigned char UART_Rx_Buf[50];
int i, UART_Rx_Index = 0, UART_Data_In = 0;

///////// Interrupt Routines //////////////////////////////
interrupt [USART_RXC] void usart_rx_isr(void) {
    UART_Data_In = 1;
    if (UART_Rx_Index <50){
    UART_Rx_Buf[UART_Rx_Index++] = UDR0;
    }    
}
////////////// TWI Interupt //////////
interrupt [TWI] void TWI_isr(void){
TWI_Slave_Int();
}          
//////////////////////////////////////////////////////////// 
void main(void)
{
    uart_init(9600);
    TWI_Init(own_address, 1, 100000);

while (1)
    { 
        
    //If receive data from UART, transfer them into TWI (Master transmit mode)
        if (UART_Data_In) {
            uart_putstring("Data received from UART : ");
            uart_putstring(UART_Rx_Buf);
            uart_putstring("\n");
            //Reset Rx_Buf
            for (i = 0; i<UART_Rx_Index;i++){
                TWI_Tx_Buf[i] = UART_Rx_Buf[i];
                UART_Rx_Buf[i] = 0;
            }
            if (TWI_Master_Send(slave_address,TWI_Tx_Buf,UART_Rx_Index))
                TWI_Error();            
            UART_Rx_Index = 0;
            UART_Data_In = 0;        
        }
    //If receive data from TWI, transfer them into UART
        if (TWI_Data_In){
            uart_putstring("Data received from TWI: ");
            uart_putstring(TWI_Rx_Buf);
            uart_putstring("\n");
            for (i = 0; i<TWI_Rx_Index;i++)
                TWI_Rx_Buf[i] = 0;                
            TWI_Rx_Index = 0;
            TWI_Data_In = 0;        
        }
    
        delay_ms(1000);   

    }
}