#include "mbed.h"
#include "Adafruit_PWMServoDriver.h"
#define MIN_PULSE_WIDTH       500
#define MAX_PULSE_WIDTH       2500
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             330
#define PI 3.14159265 

Adafruit_PWMServoDriver pwm(PB_7 ,PB_6);
InterruptIn button(USER_BUTTON);
DigitalOut led(LED1);

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
bool state = false;

void pressed()
{
    state = !state;
    led = !led;
}

void released()
{
    
}

int main() {
    //pwm.i2c_probe();
    led = 1;
    initServoDriver();
    pwm.setPWM(0, 0, pulseWidth(90));
    
    button.fall(&pressed);
    button.rise(&released);
    
    while(1)
    {
        if(state)
        {
            pwm.setPWM(0, 0, pulseWidth(0));
            
        }
        else
        {
            pwm.setPWM(0, 0, pulseWidth(60));
        }  
        
    }
}