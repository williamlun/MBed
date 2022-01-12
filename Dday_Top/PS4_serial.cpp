#include "pinDefind.h"

Serial  PS(PA_9, PA_10,115200);  // UART1 tx, rx
Serial  pc(USBTX,USBRX,115200);
    
const int PS4_buffer_size = 255;
// might need to increase buffer size for high baud rates
char PS4_tx_buffer[PS4_buffer_size+1];
char PS4_rx_buffer[PS4_buffer_size+1];
unsigned int PS4_rx_in;
unsigned int PS4_tx_limit;
bool PS4_IsCommand;


void PS4_SERIAL_INIT()
{
    PS.attach(&PS4_Rx_interrupt, Serial::RxIrq);
}

void PS4_Rx_interrupt() {
    while(PS.readable())
    {

        PS4_rx_buffer[PS4_rx_in] = PS.getc();
        
        if(PS4_rx_buffer[PS4_rx_in] == '4')
        {
            //pc.printf("4 is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);

            if(PS4_rx_buffer[PS4_rx_in-1] == 'S')
            {
                    //pc.printf("S is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                if(PS4_rx_buffer[PS4_rx_in-2] == 'P')
                {
                        //pc.printf("P is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                    PS4_IsCommand = true;
                        //pc.printf("PS4_IsCommand is true now");
                }
            }
        }
        if(PS4_rx_buffer[PS4_rx_in] == 'D' && (PS4_rx_in > 2))
        {  
                //pc.printf("D is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
            if(PS4_rx_buffer[PS4_rx_in-1] == 'N')
            {
                    //pc.printf("N is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                if(PS4_rx_buffer[PS4_rx_in-2] == 'E')
                {
                        //pc.printf("E is received and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                    if(PS4_IsCommand)
                    {
                        
                        SetCommand(PS4_rx_buffer,PS4_rx_in);
                            //pc.printf("SetCommand function is call and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                            //PC_send(PS4_rx_buffer,PS4_rx_in);
                        send(PS4_rx_buffer,PS4_rx_in);
                            //pc.printf("boardcasting function to button is call and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                    }
                    PS4_rx_in = 0;
                        //pc.printf("reset PS4_rx_in and the number of PS4_rx_in is: %03d \n",PS4_rx_in);
                    PS4_IsCommand = false;
                        //pc.printf("PS4_IsCommand reset\n");
                    return;
                }
            }
        }
        PS4_rx_in++;
        if(PS4_rx_in == PS4_buffer_size) PS4_rx_in = 0;
    }
    return;
}

void PS4_send(char transmitword[], unsigned int limit)
{
    PS4_tx_limit = limit;
    for(int i = 0; i <= PS4_tx_limit ; i++)
    {
        while (!PS.writeable());
        PS.putc(transmitword[i]);
        
    }
    PS.putc('\n');  //new line
}







