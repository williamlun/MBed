#include "pinDefind.h"

Serial  device(PA_2, PA_3,115200); // UART0 tx,rx

const int PC_buffer_size = 255;
// might need to increase buffer size for high baud rates
char PC_tx_buffer[PC_buffer_size+1];
char PC_rx_buffer[PC_buffer_size+1];
unsigned int PC_rx_in;
unsigned int PC_tx_limit;
bool PC_IsCommand;

//volatile uint16_t PC_rx_in = 0; 

void PC_SERIAL_INIT()
{
    device.attach(&PC_Rx_interrupt, Serial::RxIrq);
}
void PC_Rx_interrupt() {
    while(device.readable())
    {
        PC_rx_buffer[PC_rx_in] = device.getc();
        PC_rx_in++;
        if(PC_rx_in == PC_buffer_size) PC_rx_in = 0;
    }
}

void PC_send(char transmitword[], unsigned int limit)
{
    PC_tx_limit = limit;
    for(int i = 0; i <= PC_tx_limit ; i++)
    {
        while (!device.writeable());
        device.putc(transmitword[i]);
        
    }
    device.putc('\n');  //new line
}







