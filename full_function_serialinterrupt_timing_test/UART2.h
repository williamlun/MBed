#ifndef UART2_H
#define UART2_H



void UART_CH2_SERIAL_INIT();

void UART_CH2_send(char transmitword[], unsigned int limit);
void UART_CH2_read(char transmitword[], unsigned int limit);
void UART_CH2_Tx_interrupt();
void UART_CH2_Rx_interrupt();

char* getUART_CH2_rx_StringCommand();
uint16_t getUART_CH2_rx_CommandLength();


#endif

