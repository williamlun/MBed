#include "defind.h"

Serial UART_CH2(PA_2, PA_3, 115200);

DigitalOut led(LED1);
const int UART_CH2_buffer_size = 255;

char UART_CH2_tx_buffer[UART_CH2_buffer_size+1];
char UART_CH2_rx_buffer[UART_CH2_buffer_size+1];

char UART_CH2_STARTsign[] = "CC:";
char UART_CH2_ENDsign[] = "END";

char UART_CH2_RTS = "R";
char UART_CH2_CTS = "C";
bool UART_CH2_TxRequest = false;
bool UART_CH2_RTS = false;
bool UART_CH2_CTS = false;

char UART_CH2_rx_StringCommand[UART_CH2_buffer_size+1];
uint16_t UART_CH2_rx_CommandLength;

unsigned int UART_CH2_rx_in = 0;
unsigned int UART_CH2_tx_in = 0;
unsigned int UART_CH2_tx_out = 0;



char* getUART_CH2_rx_StringCommand()
{
    return UART_CH2_rx_StringCommand;
}
uint16_t getUART_CH2_rx_CommandLength()
{
    return UART_CH2_rx_CommandLength;
}

void UART_CH2_SERIAL_INIT()
{
    UART_CH2.attach(NULL,Serial::TxIrq);
    UART_CH2.attach(&UART_CH2_Rx_interrupt, Serial::RxIrq);
    UART_CH2_rx_StringCommand[0] = 'i';
    UART_CH2_rx_CommandLength = 1;
    UART_CH2_send("UART_CH2_SERIAL_INIT",sizeof("UART_CH2_SERIAL_INIT"));
}


void UART_CH2_send(char transmitword[], unsigned int limit)
{
    UART_CH2_TxRequest = true;
    int i = 0;
    while ((i==0) || (i < limit-1)) 
    {
        UART_CH2_tx_buffer[UART_CH2_tx_in] = transmitword[i];
        i++;
        UART_CH2_tx_in = (UART_CH2_tx_in + 1) % UART_CH2_buffer_size;
    }
    if(!RTS && CTS) 
    {
        UART_CH2.putc(UART_CH2_RTS); RTS = true; CTS = false;
    }
    return;
    
}

void UART_CH2_startSend()
{   
    bool empty;
    char temp_char;
    empty = (UART_CH2_tx_in == UART_CH2_tx_out);
    
    if (UART_CH2.writeable() && (empty)) 
    {
        temp_char = UART_CH2_tx_buffer[UART_CH2_tx_out];
        UART_CH2_tx_out = (UART_CH2_tx_out + 1) % UART_CH2_buffer_size;
        UART_CH2.putc(temp_char);
        UART_CH2.attach(&UART_CH2_Tx_interrupt, Serial::TxIrq);
    }
    return;
}

void UART_CH2_read(char transmitword[], unsigned int limit)
{
    delete[] UART_CH2_rx_StringCommand;
    for(int i = 0; i < limit; i++)
    {
        UART_CH2_rx_StringCommand[i] = transmitword[i];
         UART_CH2.putc(UART_CH2_rx_StringCommand[i]);
    }
        UART_CH2_rx_CommandLength = limit+1;
        led = !led;
}


void UART_CH2_Tx_interrupt() {
    while ((UART_CH2.writeable()) && (UART_CH2_tx_in != UART_CH2_tx_out)) 
    {
        UART_CH2.putc(UART_CH2_tx_buffer[UART_CH2_tx_out]);
        UART_CH2_tx_out = (UART_CH2_tx_out + 1) % UART_CH2_buffer_size;
    }
    if(UART_CH2_tx_in == UART_CH2_tx_out)
    {
        UART_CH2.attach(NULL,Serial::TxIrq);
        RTS = false;
        CTS = false;
    }
    return;
}

void UART_CH2_Rx_interrupt() {
    while(UART_CH2.readable())
    {
        UART_CH2_rx_buffer[UART_CH2_rx_in] = UART_CH2.getc();
        if( (RTS) && (!CTS) && (UART_CH2_rx_buffer[UART_CH2_rx_in] == UART_CH2_CTS) )
        {
                CTS = true;
                UART_CH2_rx_in = 0;
                UART_CH2_startSend();
                return;
        }

        if(UART_CH2_rx_buffer[UART_CH2_rx_in] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-2])
            if(UART_CH2_rx_buffer[UART_CH2_rx_in-1] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-3])
                if(UART_CH2_rx_buffer[UART_CH2_rx_in-2] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-4])
                {
                    UART_CH2_read(UART_CH2_rx_buffer,UART_CH2_rx_in+1);
//                    delete UART_CH2_rx_buffer;
//                    fill_n(UART_CH2_rx_buffer, UART_CH2_buffer_size+1, 0);

                    UART_CH2_rx_in = 0;
                    return;
                }
                
        UART_CH2_rx_in++;
        if(UART_CH2_rx_in == UART_CH2_buffer_size) UART_CH2_rx_in = 0;
    }
    return;
}
