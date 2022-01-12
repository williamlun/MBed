#include "pinDefind.h"
#include "math.h"
#define PI 3.14159265 
#include "Adafruit_PWMServoDriver.h"
#include"mbed.h"
#define MIN_PULSE_WIDTH       500
#define MAX_PULSE_WIDTH       2500
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

DigitalOut myled(LED1);
InterruptIn button(USER_BUTTON);

DigitalOut shooter_in(PC_2);
DigitalOut shooter_out(PC_3);
DigitalOut loader_updown(PC_0);
DigitalOut loader_turn(PC_5);
DigitalOut finger(PB_0);
DigitalOut shoot(PC_1);

/*
    George PWM 0 up 128 give 89
    lock shit PWM 2  start 113  down 168
    push shit PWM 4 start 284 down 78
    pick up shit PWM 6 start 73 down 123
*/
Ticker servoCombo;
Ticker updownServo;

int servoComboCounter = 0;

bool Bloader_updown = true;
bool Bloader_turn = true;
bool Bsuck = false;


bool servomoveshit = true;
bool servoupdown = true;

bool dosome = false;
bool upservo = false;

void F_servoCombo();
void F_servoUpdown();


Adafruit_PWMServoDriver pwm(PB_7,PB_6);
 
void setServoPulse(uint8_t n, float pulse) {
    pwm.setPWM(n, 0, pulse);
}
 
void initServoDriver() {
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
}
 

float Servomap(float in, float inMin, float inMax, float outMin, float outMax) {
  // check it's within the range
  if (inMin<inMax) { 
    if (in <= inMin) 
      return outMin;
    if (in >= inMax)
      return outMax;
  } else {  // cope with input range being backwards.
    if (in >= inMin) 
      return outMin;
    if (in <= inMax)
      return outMax;
  }
  // calculate how far into the range we are
  float scale = (in-inMin)/(inMax-inMin);
  // calculate the output.
  return outMin + scale*(outMax-outMin);
}
 
int pulseWidth(double angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = Servomap(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  //Serial.println(analog_value);
  return analog_value;
}

int pulseWidth320(double angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = Servomap(angle, 0, 320, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  //Serial.println(analog_value);
  return analog_value;
}



bool suckerState = false;
bool servoState = false;   
uint8_t servoState2 = 0;        
bool servoState3 = false;   

static double SPEED_COEFFICIENT = 2;
static double SPEED_POWER_FACTOR = 1;     
char counter = 0;
int connectflag = 0;
double v0,v1,v2,vt,vnt, wt;

float d = 0.349;
float wheelR = 0.0762; //152.4
float map(float in, float inMin, float inMax, float outMin, float outMax) {
  // check it's within the range
  if (inMin<inMax) { 
    if (in <= inMin) 
      return outMin;
    if (in >= inMax)
      return outMax;
  } else {  // cope with input range being backwards.
    if (in >= inMin) 
      return outMin;
    if (in <= inMax)
      return outMax;
  }
  // calculate how far into the range we are
  float scale = (in-inMin)/(inMax-inMin);
  // calculate the output.
  return outMin + scale*(outMax-outMin);
}

bool state = false;
void pressed()
{
    state = !state;
}

void F_servoCombo()
{
    
    if(servoComboCounter == 0)
    {
        pwm.setPWM(2, 0, pulseWidth(180));
    }
    else if(servoComboCounter == 1)
    {
        pwm.setPWM(2, 0, pulseWidth(97));  
    }
    else if(servoComboCounter == 2)
    {
        pwm.setPWM(4, 0, pulseWidth(180));   
    }
    else if(servoComboCounter == 3)
    {
        pwm.setPWM(4, 0, pulseWidth(0)); 
        servoCombo.detach();
          
    }
    servoComboCounter++;
    
}
void F_servoUpdown()
{
    
}

int main() {
    //PC_send("hello", 5);
        
    PC_SERIAL_INIT();
    PS4_SERIAL_INIT();
    UART_CH2_SERIAL_INIT();
    initServoDriver();
    
    shooter_in = 0;
    shooter_out = 0;
    loader_updown = 0;
    loader_turn = 0;
    shoot = 0;

    
    //pwm.setPWM(15, 0, pulseWidth(70));
    //pwm.setPWM(1, 0, pulseWidth(180));
    
    pwm.setPWM(0, 0, pulseWidth(0));
    pwm.setPWM(2, 0, pulseWidth(113));
    pwm.setPWM(4, 0, pulseWidth320(284));
    pwm.setPWM(6, 0, pulseWidth(73));
    button.fall(&pressed);
    
    while (1) {
        checkFlatLoop();
        if(dosome)
        {
            for(int i = 120; i >90 ;i--)
            {
                //printf("%d\r\n",i);
                pwm.setPWM(0, 0, pulseWidth(i));
                wait(0.015);
            }
            shooter_in = 0;
            dosome = false;
        }
    }
}
    //////////////////////////////////END of MAIN PROGRAM////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////   
    /////////////////////////////////////////////////////////////////////////////////////////
    void    fall_func_CommandChange(){}
    void    fall_func_LH(){}
    void    fall_func_RH(){}
    void    fall_func_L2(){}
    void    fall_func_R2(){}
    void    fall_func_TRI(){}
    void    fall_func_CIR(){shooter_in = 0;}
    void    fall_func_CRO()
            {
                
            }
    void    fall_func_SQU(){shooter_out = 0; myled = 0;}
    void    fall_func_UP(){}
    void    fall_func_RIGHT(){}
    void    fall_func_DOWN(){}
    void    fall_func_LEFT(){}
    void    fall_func_L1(){}
    void    fall_func_L3(){}
    void    fall_func_R1(){}
    void    fall_func_R3(){}
    void    fall_func_SHARE(){shoot = 0;}
    void    fall_func_OPTIONS(){}
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    void    rise_func_CommandChange(){}
    void    rise_func_LH(){}
    void    rise_func_RH(){}
    void    rise_func_L2()
            {
                 //servoComboCounter = 0;
                 //servoCombo.attach(&F_servoCombo, 0.5);
                 
                 if(servomoveshit)
                {
                    pwm.setPWM(2, 0, pulseWidth(168));
                    pwm.setPWM(4, 0, pulseWidth320(78));
                    loader_turn = 0;
                    Bloader_turn = true;
                    servomoveshit = false;
                }
                else
                {
                    pwm.setPWM(2, 0, pulseWidth(113));
                    pwm.setPWM(4, 0, pulseWidth320(284));
                    servomoveshit = true;
                }
                
            }
    void    rise_func_R2()
            {
                if(Bloader_updown)
                {
                    if(Bloader_turn)
                    {
                        loader_turn = 1;
                        shooter_in = 1;
                        Bloader_turn = false;
                    }
                    else
                    {
                        loader_turn = 0;
                        shooter_in = 0;
                        Bloader_turn = true;
                    }
                }
            }
    void    rise_func_TRI()
            {
                if(Bloader_turn)
                {
                    if(Bloader_updown)
                    {
                        loader_updown = 1;
                        Bloader_updown = false;
                    }else 
                    {
                        loader_updown = 0;
                        Bloader_updown = true;
                    }
                }

            }
    void    rise_func_CIR(){
                shooter_in = 1;
               
            }
    void    rise_func_CRO(){
                
                if(servoupdown)
                {   
                    
                    shooter_in = 1;
                    pwm.setPWM(0, 0, pulseWidth(128));
                    wait(0.25);
                    shooter_in = 0;
                    servoupdown = false;
                }
                else
                {
                    dosome = true;
                    shooter_in = 1;
                    //pwm.setPWM(0, 0, pulseWidth(91));
                    servoupdown = true;
                }
            }
    void    rise_func_SQU()
            {
                shooter_out = 1;
                myled = 1;
            }
    void    rise_func_UP(){}
    void    rise_func_RIGHT(){}
    void    rise_func_DOWN(){}
    void    rise_func_LEFT(){}
    void    rise_func_L1(){}
    void    rise_func_L3(){}
    void    rise_func_R1(){}
    void    rise_func_R3(){}
    void    rise_func_SHARE(){if(servomoveshit) shoot = 1;}
    void    rise_func_OPTIONS()
            {
                if(Bsuck)
                {
                    pwm.setPWM(6, 0, pulseWidth(73));
                    Bsuck = false;
                }
                else
                {
                     pwm.setPWM(6, 0, pulseWidth(123));
                    Bsuck = true;   
                }
            }
    
    /////////////////////////////////////////////////////////////////////////////
    
    
    