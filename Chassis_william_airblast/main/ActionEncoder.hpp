/*
 * ActionEncoder.hpp
 *
 *  Created on: 7 Mar 2018
 *      Author: tung
 */

#ifndef INCLUDE_ACTIONENCODER_HPP_
#define INCLUDE_ACTIONENCODER_HPP_

#include <stdint.h>
//#include <Serial.hpp>

class ActionEncoder
{
private:
    uint8_t count,i,done;
    union{
        uint8_t data[24];
        float val[6];
    }posture;
    float xangle,yangle,zangle,pos_x,pos_y,angleSpeed,d_angle;
    float temp_zangle;
    bool newDataArrived;
public:
    ActionEncoder();
    void readEncoder(char);
    bool updated();
    float getXangle();
    float getYangle();
    float getZangle();
    float getXpos();
    float getYpos();
    float getAngleSpeed();
    char * reset();
    char * calibrate();
    bool isAlive();
    bool newDataAvailable();
    uint32_t LastRead;

};







#endif /* INCLUDE_ACTIONENCODER_HPP_ */
