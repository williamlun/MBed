#ifndef PC_SERIAL_H
#define PC_SERIAL_H

//Serial  device(PA_2, PA_3); // UART0 tx,rx

void PC_SERIAL_INIT();
void PC_Rx_interrupt();
void PC_send(char transmitword[], unsigned int limit);


#endif



