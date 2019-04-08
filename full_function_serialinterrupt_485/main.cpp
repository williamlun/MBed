#include "defind.h"

InterruptIn button(USER_BUTTON);

void pressed()
{
    
    UART_CH2_send(getUART_CH2_rx_StringCommand(),getUART_CH2_rx_CommandLength());

}

void released()
{
    
}



int main() 
{
    UART_CH2_SERIAL_INIT();
    
    button.fall(&pressed);
    button.rise(&released);
    while(1)
    {
        
        
    }
}
