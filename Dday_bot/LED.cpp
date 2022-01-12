#include "robot.h"

  
//*DEFINE INSTRUCTION PIN
DigitalOut      led1(LED1);
DigitalOut      led2(LED2);

void LED_1_TOGGLE()
{
    led1 = !led1;
}

void LED_1_ON()
{
    led1 = 1;    
}

void LED_1_OFF()
{
    led1 = 0;   
}
