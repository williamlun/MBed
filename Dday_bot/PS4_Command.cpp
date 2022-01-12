#include "pinDefind.h"
#include "robot.h"
//readable ver.

    //void init()
//    {
//        for(int i = 0; i < 19; i++)
//            CommandFlat[i] = false;  
//        StringCommand = new char[255];
//    }

    char *StringCommand;
    uint16_t CommandLength;
    bool old_CommandFlat[19];
    bool CommandFlat[19];
    uint16_t LX,LY,RX,RY,LA2,RA2;
    ///ken variable
    int tester = 0;
    
    ////////////////////////////////////////////////////////////
    //////////////////////3 wheel kinematic/////////////////////
    ////////////////////////////////////////////////////////////
bool allFlag = false;
float tarvt = 0,tarvnt = 0,tarwt = 0;
int v0=1,v1=1,v2 =1;
float v0f,v1f,v2f;
float vt,vnt,wt;
float d = 0.35195;
float wheelR = 0.1524/2;
float PI = 3.1416;



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


void updateki(float newvt, float newvnt, float newwt){
    vt = (-1)*newvt;
    vnt = (-1)*newvnt;
    wt = (-1)*newwt;
    
    v0f = (-1)*(sin(PI/3))*vt + cos(PI/3)*vnt + d*wt;
    v1f = (-1)*vnt + d*wt;
    v2f = sin(PI/3)*vt + cos(PI/3)*vnt + d*wt;
    v0f = v0f*60/(wheelR*2*PI);
    v1f = v1f*60/(wheelR*2*PI);
    v2f = v2f*60/(wheelR*2*PI);
    
    v0 = (int)(v0f*26);
    v1 = (int)(v1f*26);
    v2 = (int)(v2f*26);
}

    
     void SEND(){
         
         PC_SEND(v0);
         PC_SEND("\r\n");
         PC_SEND(v1);
         PC_SEND("\r\n");
         PC_SEND(v2);
         PC_SEND("\r\n");
         }
    
    int getLX(){ return (int)LX; }
    int getLY(){ return (int)LY; }
    int getRX(){ return (int)RX; }
    int getRY(){ return (int)RY; }
    int getLA2(){ return (int)LA2; }
    int getRA2(){ return (int)RA2; }
    
    void (*ps4high_func_ptr[19])() = {
        high_func_CommandChange,
        high_func_LH ,
        high_func_RH ,
        high_func_L2 ,
        high_func_R2 ,
        high_func_TRI ,
        high_func_CIR ,
        high_func_CRO,
        high_func_SQU,
        high_func_UP,
        high_func_RIGHT,
        high_func_DOWN,
        high_func_LEFT,
        high_func_L1,
        high_func_L3,
        high_func_R1,
        high_func_R3,
        high_func_SHARE,
        high_func_OPTIONS
    };
    
    void (*ps4low_func_ptr[19])() = {
        low_func_CommandChange,
        low_func_LH ,
        low_func_RH ,
        low_func_L2 ,
        low_func_R2 ,
        low_func_TRI ,
        low_func_CIR ,
        low_func_CRO,
        low_func_SQU,
        low_func_UP,
        low_func_RIGHT,
        low_func_DOWN,
        low_func_LEFT,
        low_func_L1,
        low_func_L3,
        low_func_R1,
        low_func_R3,
        low_func_SHARE,
        low_func_OPTIONS
    };
    
    void (*ps4fall_func_ptr[19])() = {
        fall_func_CommandChange,
        fall_func_LH ,
        fall_func_RH ,
        fall_func_L2 ,
        fall_func_R2 ,
        fall_func_TRI ,
        fall_func_CIR ,
        fall_func_CRO,
        fall_func_SQU,
        fall_func_UP,
        fall_func_RIGHT,
        fall_func_DOWN,
        fall_func_LEFT,
        fall_func_L1,
        fall_func_L3,
        fall_func_R1,
        fall_func_R3,
        fall_func_SHARE,
        fall_func_OPTIONS
    };
    
    void (*ps4rise_func_ptr[19])() = {
        rise_func_CommandChange,
        rise_func_LH ,
        rise_func_RH ,
        rise_func_L2 ,
        rise_func_R2 ,
        rise_func_TRI ,
        rise_func_CIR ,
        rise_func_CRO,
        rise_func_SQU,
        rise_func_UP,
        rise_func_RIGHT,
        rise_func_DOWN,
        rise_func_LEFT,
        rise_func_L1,
        rise_func_L3,
        rise_func_R1,
        rise_func_R3,
        rise_func_SHARE,
        rise_func_OPTIONS
    };


    
    uint16_t char2dec(char a,char b, char c)
    {
        return (a & 0x0F)*100 + (b & 0x0F)*10 + (c & 0x0F);
    }
    
    void SetCommand(char InputCommand[],uint16_t limit)
    {
        delete StringCommand;
        StringCommand =  new char[255];
        for(int i = 0; i < limit; i++)
            StringCommand[i] = InputCommand[i];
        CommandFlat[CommandChange] = true;
        CommandLength = limit;
        
    }
    
    void SetFlat()
    {
        
        for(int i = 0;i<19;i++)
        {
            old_CommandFlat[i] = CommandFlat[i];
            CommandFlat[i] = false;
        }
            
            
        for(int i = 0;i < CommandLength ; i++)
        {
            switch(StringCommand[i])
            {
                case 'L':
                    switch(StringCommand[i+1])
                    {
                        case '1':
                            CommandFlat[L1] = true;
                        break;
                        
                        case '2':
                            CommandFlat[L2] = true;
                            LA2 = char2dec(StringCommand[i+4],StringCommand[i+5],StringCommand[i+6]);
                        break;
                        
                        case '3':
                            CommandFlat[L3] = true;
                        break;
                        
                        case 'e':
                            if(StringCommand[i+7] == 'X'){
                                LX = char2dec(StringCommand[i+10],StringCommand[i+11],StringCommand[i+12]);
                                CommandFlat[LH] = true;
                            }else if (StringCommand[i+7] == 'Y'){
                                LY = char2dec(StringCommand[i+10],StringCommand[i+11],StringCommand[i+12]);
                                CommandFlat[LH] = true;
                            }
                        break;
                        
                        case 'E':
                            if(StringCommand[i+2] == 'F')
                                CommandFlat[LEFT] = true;
                        break;
                        
                        default:
                        break;
                    }
                break;
                
                case 'R':
                    switch(StringCommand[i+1])
                    {
                        case '1':
                            CommandFlat[R1] = true;
                        break;
                        
                        case '2':
                            CommandFlat[R2] = true;
                            RA2 = char2dec(StringCommand[i+4],StringCommand[i+5],StringCommand[i+6]);
                        break;
                        
                        case '3':
                            CommandFlat[R3] = true;
                        break;
                        
                        case 'i':
                            if(StringCommand[i+8] == 'X'){
                                RX = char2dec(StringCommand[i+11],StringCommand[i+12],StringCommand[i+13]);
                                CommandFlat[RH] = true;
                            }else if (StringCommand[i+8] == 'Y'){
                                RY = char2dec(StringCommand[i+11],StringCommand[i+12],StringCommand[i+13]);
                                CommandFlat[RH] = true;
                            }
                        break;
                        
                        case 'I':
                            if(StringCommand[i+2] == 'G')
                                CommandFlat[RIGHT] = true;
                        break;
                        
                        default:
                        break;
                    }
                break;
                
                case 'U':
                    if(StringCommand[i+1] == 'P')
                        CommandFlat[UP] = true;
                break;
                
                case 'D':
                    if(StringCommand[i+1] == 'O')
                        CommandFlat[DOWN] = true;
                break;
                
                case 'T':
                    if(StringCommand[i+1] == 'R')
                        CommandFlat[TRI] = true;
                break;
                
                case 'C':
                    if(StringCommand[i+1] == 'R')
                        CommandFlat[CRO] = true;
                    else if(StringCommand[i+1] == 'I')
                        CommandFlat[CIR] = true;
                break;
                
                case 'S':
                    if(StringCommand[i+1] == 'Q')
                        CommandFlat[SQU] = true;
                    else if (StringCommand[i+1] == 'H')
                        CommandFlat[SHARE] = true;
                break;
                
                case 'O':
                    if(StringCommand[i+1] == 'P')
                        CommandFlat[OPTIONS] = true;
                break;
                
                default:
                break;
            }
        }
    }
    void checkFlatLoop()
    {
        if(CommandFlat[CommandChange] != old_CommandFlat[CommandChange] || CommandFlat[CommandChange])
            SetFlat();
        allFlag = false;
        
        for(int i = 0; i < 19; i++)
        {
            if(CommandFlat[i] && !old_CommandFlat[i])
                (*ps4rise_func_ptr[i])();
            if(!CommandFlat[i] && old_CommandFlat[i])
                (*ps4fall_func_ptr[i])(); 
            if(CommandFlat[i])
            {
                (*ps4high_func_ptr[i])();
                allFlag = true;
            }
        }
        if(!allFlag) allFlagDown();
    }
    
    ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
    
    
    ///////////////////////////////////////////
    /*           HIGH FUNCTION              */
    //////////////////////////////////////////
    void    high_func_CommandChange(){}
    void    high_func_LH()
            {

            }
    void    high_func_RH(){}
    void    high_func_L2(){}
    void    high_func_R2(){}
    void    high_func_TRI(){}
    void    high_func_CIR(){}
    void    high_func_CRO(){}
    void    high_func_SQU(){}
    void    high_func_UP(){}
    void    high_func_RIGHT(){}
    void    high_func_DOWN(){}
    void    high_func_LEFT(){}
    void    high_func_L1(){}
    void    high_func_L3(){}
    void    high_func_R1(){}
    void    high_func_R3(){}
    void    high_func_SHARE(){}
    void    high_func_OPTIONS(){}
    
    ///////////////////////////////////////////
    /*           LOW FUNCTION                */
    ///////////////////////////////////////////
    
    void    low_func_CommandChange(){}
    void    low_func_LH(){}
    void    low_func_RH(){}
    void    low_func_L2(){}
    void    low_func_R2(){}
    void    low_func_TRI(){}
    void    low_func_CIR(){}
    void    low_func_CRO(){}
    void    low_func_SQU(){}
    void    low_func_UP(){}
    void    low_func_RIGHT(){}
    void    low_func_DOWN(){}
    void    low_func_LEFT(){}
    void    low_func_L1(){}
    void    low_func_L3(){}
    void    low_func_R1(){}
    void    low_func_R3(){}
    void    low_func_SHARE(){}
    void    low_func_OPTIONS(){}


    
    ///////////////////////////////////////////
    /*            FALL FUNCTION              */
    ///////////////////////////////////////////
    
    void    fall_func_CommandChange(){}
    void    fall_func_LH(){}
    void    fall_func_RH(){}
    void    fall_func_L2(){}
    void    fall_func_R2(){}
    void    fall_func_TRI(){}
    void    fall_func_CIR(){}
    void    fall_func_CRO(){}
    void    fall_func_SQU(){}
    void    fall_func_UP()
            {
                tarvt = tarvt - 1.42;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    fall_func_RIGHT()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0.6;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP); 
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    fall_func_DOWN()
            {
                tarvt = tarvt + 0.7;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);                 
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);                 
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);                 
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);                 
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);         
            }
    void    fall_func_LEFT()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt +0.6;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);                 
                SET_TARGET_VELOCITY(2, v1);                 
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);                 
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);                 
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);         
            }
    void    fall_func_L1()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0;
                tarwt = tarwt - 1.95;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                 
                SET_TARGET_VELOCITY(2, v1);
                 
                SET_TARGET_VELOCITY(3, v2);
                 wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                 
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                 
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
         
    }
    void    fall_func_L3()
            {
                tarvt = tarvt - 0.43;
                tarvnt = tarvnt + 0.43;
                tarwt = tarwt - 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    fall_func_R1()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 1.95;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
         
            }
    void    fall_func_R3()
            {
                tarvt = tarvt - 0.43;
                tarvnt = tarvnt - 0.43;
                tarwt = tarwt - 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    fall_func_SHARE(){}
    void    fall_func_OPTIONS(){}
    
    ////////////////////////////////////////////
    /*           RISE FUNCTION                */
    ////////////////////////////////////////////
    
    void    rise_func_CommandChange(){}
    void    rise_func_LH()
            {   
            }
    void    rise_func_RH()
            {
            }
    void    rise_func_L2(){}
    void    rise_func_R2(){}
    void    rise_func_TRI(){}
    void    rise_func_CIR(){}
    void    rise_func_CRO(){}
    void    rise_func_SQU(){}
    void    rise_func_UP()
            {
                tarvt = tarvt + 1.42;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);   
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_RIGHT()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt + 0.6;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_DOWN()
            {
                tarvt = tarvt - 0.7;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_LEFT()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0.6;
                tarwt = tarwt + 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_L1()
            {   
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0;
                tarwt = tarwt + 1.95;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_L3()
            {
                tarvt = tarvt + 0.43;
                tarvnt = tarvnt - 0.43;
                tarwt = tarwt - 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_R1()
            {
                tarvt = tarvt + 0;
                tarvnt = tarvnt - 0;
                tarwt = tarwt - 1.95;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_R3()
            {
                tarvt = tarvt + 0.43;
                tarvnt = tarvnt + 0.43;
                tarwt = tarwt - 0;
                updateki((float)tarvt,(float)tarvnt,(float)tarwt);
                SET_TARGET_VELOCITY(1, v0);
                SET_TARGET_VELOCITY(2, v1);
                SET_TARGET_VELOCITY(3, v2);
                wait(0.001);
                RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
                RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
            }
    void    rise_func_SHARE(){}
    void    rise_func_OPTIONS(){}
    
    


    void allFlagDown()
        {

                
        }


