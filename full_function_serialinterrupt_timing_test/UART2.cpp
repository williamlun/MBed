#include "defind.h"

Serial UART_CH2(PA_2, PA_3, 115200);

DigitalOut led(LED1);
const int UART_CH2_buffer_size = 255;

char UART_CH2_tx_buffer[UART_CH2_buffer_size+1];
char UART_CH2_rx_buffer[UART_CH2_buffer_size+1];

char UART_CH2_STARTsign[] = "CC:";
char UART_CH2_ENDsign[] = "END";

char UART_CH2_RTSmessage = 'R';
char UART_CH2_CTSmessage = 'C';
bool UART_CH2_TxRequest = false;
bool UART_CH2_RTS = false;
bool UART_CH2_CTS = true;

char UART_CH2_rx_StringCommand[UART_CH2_buffer_size+1];
uint16_t UART_CH2_rx_CommandLength;

unsigned int UART_CH2_rx_in = 0;
unsigned int UART_CH2_tx_in = 0;
unsigned int UART_CH2_tx_out = 0;

Ticker waitCTStoRTS;
Ticker waitCTStoDATA;
Timeout backoff;
Timeout channelDelay;


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
    //UART_CH2_send("UART_CH2_SERIAL_INIT",sizeof("UART_CH2_SERIAL_INIT"));
}
void UART_CH2_waitCTS_toRTS()
{
    led = !led;
    if(UART_CH2_CTS)
    {
        waitCTStoRTS.detach();
        UART_CH2.putc(UART_CH2_RTSmessage);
        UART_CH2_RTS = true;
    }

}
void UART_CH2_waitCTS_toData()
{
    if(UART_CH2_RTS && UART_CH2_CTS)
    {
        waitCTStoDATA.detach();
        UART_CH2_startSend();
    }
    else if(UART_CH2_CTS)
    {
        UART_CH2.putc(UART_CH2_RTSmessage);
    }
    
}
void UART_CH2_backoff()
{
    if(UART_CH2_CTS)
    {
        UART_CH2.putc(UART_CH2_RTSmessage);
        UART_CH2_RTS = true;
    }
}
void UART_CH2_send(char transmitword[], unsigned int limit)
{
    int i = 0;
    led = 0;
    while ((i==0) || (i < limit-1)) 
    {
        UART_CH2_tx_buffer[UART_CH2_tx_in] = transmitword[i];
        i++;
        UART_CH2_tx_in = (UART_CH2_tx_in + 1) % UART_CH2_buffer_size;
    }
    if(!UART_CH2_CTS) 
    {
        //wait CTS to send RTS
        waitCTStoRTS.attach_us(&UART_CH2_waitCTS_toRTS,2500);
    }
    if(UART_CH2_CTS && UART_CH2_RTS) UART_CH2_startSend();
    if(UART_CH2_CTS && !UART_CH2_RTS) 
    {
        UART_CH2.putc(UART_CH2_RTSmessage);
        UART_CH2_RTS = true;
        UART_CH2_CTS = false;
        waitCTStoDATA.attach_us(&UART_CH2_waitCTS_toData, 2500);
    }
    return;
    
}

void UART_CH2_send()
{
    if(!UART_CH2_CTS) 
    {
        //wait CTS to send RTS
        waitCTStoRTS.attach_us(&UART_CH2_waitCTS_toRTS,2500);
    }
    if(UART_CH2_CTS && UART_CH2_RTS) UART_CH2_startSend();
    if(UART_CH2_CTS && !UART_CH2_RTS) 
    {
        UART_CH2.putc(UART_CH2_RTSmessage);
        UART_CH2_RTS = true;
        waitCTStoDATA.attach_us(&UART_CH2_waitCTS_toData, 2500);
    }
    return;
}



void UART_CH2_startSend()
{   
    led = 0;
    bool empty;
    char temp_char;
    empty = (UART_CH2_tx_in != UART_CH2_tx_out);
     
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
        UART_CH2_RTS = false;
        UART_CH2_CTS = true;
        UART_CH2_TxRequest = false;

    }
    return;
}


void UART_CH2_receiveCTS2data()
{
    channelDelay.detach();
    UART_CH2_CTS = true;
    led = 0;
    UART_CH2_waitCTS_toData();
}

void UART_CH2_receiveRTS2CTS()
{
    channelDelay.detach();
    UART_CH2.putc(UART_CH2_CTSmessage);
    UART_CH2_rx_in = 0;
    UART_CH2_CTS = false;
}

void UART_CH2_receiveEND2CTS()
{
    channelDelay.detach();
    UART_CH2_CTS = true;
}

void UART_CH2_Rx_interrupt() {
    while(UART_CH2.readable())
    {
        
        UART_CH2_rx_buffer[UART_CH2_rx_in] = UART_CH2.getc();
        if( (UART_CH2_RTS) && (UART_CH2_rx_buffer[UART_CH2_rx_in] == UART_CH2_CTSmessage) )
        {
                channelDelay.attach_us(&UART_CH2_receiveCTS2data, 2500);
                return;
        }
        if((UART_CH2_rx_buffer[UART_CH2_rx_in] == UART_CH2_RTSmessage) && UART_CH2_CTS)
        {
            if(UART_CH2_RTS) 
            {
                /*
                UART_CH2_RTS = false;
                 UART_CH2_rx_in = 0;
                backoff.attach_us(&UART_CH2_send,((rand() % 901) +100));
                */
                return;
            }
            else if(!UART_CH2_RTS)
            {
                channelDelay.attach_us(&UART_CH2_receiveRTS2CTS, 2500);
                return;
            }
        }

        if(UART_CH2_rx_buffer[UART_CH2_rx_in] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-2])
            if(UART_CH2_rx_buffer[UART_CH2_rx_in-1] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-3])
                if(UART_CH2_rx_buffer[UART_CH2_rx_in-2] == UART_CH2_ENDsign[sizeof(UART_CH2_ENDsign)-4])
                {
                    UART_CH2_read(UART_CH2_rx_buffer,UART_CH2_rx_in+1);
//                    delete UART_CH2_rx_buffer;
//                    fill_n(UART_CH2_rx_buffer, UART_CH2_buffer_size+1, 0);

                    UART_CH2_rx_in = 0;
                    channelDelay.attach_us(&UART_CH2_receiveEND2CTS, 2500);
                    return;
                }
                
        UART_CH2_rx_in++;
        if(UART_CH2_rx_in == UART_CH2_buffer_size) UART_CH2_rx_in = 0;
    }
    return;
}
