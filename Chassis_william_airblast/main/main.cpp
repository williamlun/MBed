#include "mbed.h"
#include "actiondrv.h"

#include "millis.h"

/*
 * ActionEncoder.cpp
 *
 *  Created on: 7 Mar 2018 https://bakerfranke.github.io/codePrint/
 *      Author: Sam Shum
 */

#include "ActionEncoder.hpp"
#include "Timer.h"



///////////////////////////
//Serial Action(D8,D2 ); // tx, rx
Serial Action(PA_9,  PA_10 );
Serial pc(USBTX, USBRX);



union {
    uint8_t data[24];
    float val[6];
} posture;
int count=0;
int i=0;
int done=0;
float xangle=0;
float yangle=0;
float zangle=0;
float d_angle=0;
float pos_x=0;
float pos_y=0;
float angleSpeed=0;
float temp_zangle=0;
int   LastRead=0;
bool newDataArrived=false;

char buffer[8];
/////////////////////////

//Serial pc(USBTX, USBRX);
char counter = 0;
actionDrv action1(1);
actionDrv action2(3);
actionDrv action3(2);


int motor1 = 0;
int motor2 = 0;
int motor3 = 0;

float pi = 3.14159265;
double pi_div_3 = 1.04719755;
float d = 0.35195;//0.43;
float wheelR = 0.1524/2; //4 inch wheel
float gear = 12.5;

Ticker motor_updater;

Ticker odom_updater;
//////////////////////////////////////
int point_counter=0;

struct point_info
{
   float required_x;
   float required_y;
   float required_w;
   float required_tolerance_x;
   float required_tolerance_y;
   float required_tolerance_w;
   float required_speed_max_x;
   float required_speed_max_y;
   float required_speed_max_w;
   float pid_on;
};




struct point_info points[100];
/////////////////
float local_vel_x = 0;
float local_vel_y = 0;
float local_vel_w = 0;
    
float global_vel_x = 0;
float global_vel_y = 0;

////////////////////////////////////
float now_x=0;
float now_y=0;
float now_w=0;


float   target_x    = points[0].required_x;  //  +  startup_offset_x_encoder;
float   target_y    = points[0].required_y;  //  +  startup_offset_y_encoder;
float   target_w    = points[0].required_w *pi/float(180);   // -  startup_offset_w_encoder;
bool    pid_on      = points[0].pid_on;
//points[0] = (point_info){.required_x = 0,.required_y = 0, .required_w = 0, .required_tolerance_x = 0.05, .required_tolerance_y = 0.05, .required_tolerance_w = 0.01, .required_speed_max_x = 0.3, .required_speed_max_y = 0.3, .required_speed_max_w = 0.01};

float tolerance_x = 0.08;
float tolerance_y = 0.08;
float tolerance_w = 0.1;

float speed_max_x = 0.4;
float speed_max_y = 0.4;
float speed_max_w = 0.4;

/*    //Setting 1 slow
float tolerance_x = 0.08;
float tolerance_y = 0.08;
float tolerance_w = 0.1;

float speed_max_x = 0.3;
float speed_max_y = 0.3;
float speed_max_w = 0.6;
*/

/*
float tolerance_x = points[0].required_tolerance_x;
float tolerance_y = points[0].required_tolerance_y;
float tolerance_w = points[0].required_tolerance_w;

float speed_max_x = points[0].required_speed_max_x;
float speed_max_y = points[0].required_speed_max_y;
float speed_max_w = points[0].required_speed_max_w;
*/

long odom_last_read= millis();

/////////////////////////////////////
const float RATE = 0.08;//0.18;
//////////////////////////

///////////////////////////
float encoder_2_global_angle = 90;         //encoder coordinate system + 30 degree    =>  global coordinate system
//float encoder_2_global_x     =   0.34;    //encoder to center distance  in x   (tung)
//float encoder_2_global_y     =   -0.235;     //encoder to center distance  in y   (tung)


float encoder_2_global_x     = 0.1221206;//0.125;    //encoder to center distance  in x   (tung)
float encoder_2_global_y     = 0.1081795;//0.37;    //encoder to center distance  in y   (tung)
////////////////////TUNG////////////////


float transformed_real_now_x=0;
float transformed_real_now_y=0;
float transformed_real_now_w=0;


float startup_offset_x_encoder =0;
float startup_offset_y_encoder =0;
float startup_offset_w_encoder=0;



float encoder_to_center = sqrt( ( encoder_2_global_x  * encoder_2_global_x )  + ( encoder_2_global_y  *  encoder_2_global_y ) );

//#########################//
float encoder2local_angle = encoder_2_global_angle *pi/float(180);
float encoder_position_angle =( ( 211.54)  ) / float(180) * pi ;   //90 +  angle to encoder location
float r = sqrt( ( encoder_2_global_x  * encoder_2_global_x )  + ( encoder_2_global_y  *  encoder_2_global_y ) );   //encoder to center radius


void calculatePos(float _X,float _Y,float _A)
{
    float zangle  =  _A-   360 * int(_A / 360);
    float zrangle =  zangle *pi/float(180);    //degree 2 rad
    
    float tx = ( ( _X / float(1000) ) * cos( -encoder2local_angle) )  -  (  ( _Y / float(1000) )   *  sin( -encoder2local_angle) );
    float ty = ( ( _X / float(1000) ) * sin( -encoder2local_angle) )  +  (  ( _Y / float(1000) )   *  cos( -encoder2local_angle) );
    
    float s  = copysign( sqrt(  2*( r*r )  -  2*(r*r)*cos(zrangle)  )    , zrangle );
    
    float x_bias = s * cos( zrangle / 2 );
    float y_bias = s * sin( zrangle / 2 );
    
    float x_tbias = ( x_bias ) * ( cos( encoder_position_angle) )  - ( y_bias ) * ( sin( encoder_position_angle ) )    ;
    float y_tbias = ( x_bias ) * ( sin( encoder_position_angle) )  + ( y_bias ) * ( cos( encoder_position_angle ) )    ;
    
    
   transformed_real_now_x = tx + y_tbias   - startup_offset_x_encoder; //-
   transformed_real_now_y = ty - x_tbias   - startup_offset_y_encoder;
    
    //transformed_real_now_x = tx + y_tbias   - startup_offset_x_encoder;  //+
    //transformed_real_now_y = ty - x_tbias   - startup_offset_y_encoder;
    
    
    transformed_real_now_w=   _A *pi/float(180)                                                                        -  startup_offset_w_encoder;
    
    
}





///////////////////////



float x_PID_P = 1.5;  //0.4    //2
float y_PID_P = 1.5;  //0.4    //2
float w_PID_P = 2.5;  //0.4    //1

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

//////////////////////////////
void start_calculatePos(float _X,float _Y,float _A)
{
    float zangle  =  _A-   360 * int(_A / 360);
    float zrangle =  zangle *pi/float(180);    //degree 2 rad
    
    float tx = ( ( _X / float(1000) ) * cos( -encoder2local_angle) )  -  (  ( _Y / float(1000) )   *  sin( -encoder2local_angle) );
    float ty = ( ( _X / float(1000) ) * sin( -encoder2local_angle) )  +  (  ( _Y / float(1000) )   *  cos( -encoder2local_angle) );
    
    float s  = copysign( sqrt(  2*( r*r )  -  2*(r*r)*cos(zrangle)  )    , zrangle );
    
    float x_bias = s * cos( zrangle / 2 );
    float y_bias = s * sin( zrangle / 2 );
    
    float x_tbias = ( x_bias ) * ( cos( encoder_position_angle) )  - ( y_bias ) * ( sin( encoder_position_angle ) )    ;
    float y_tbias = ( x_bias ) * ( sin( encoder_position_angle) )  + ( y_bias ) * ( cos( encoder_position_angle ) )    ;
    
    
    startup_offset_x_encoder = tx + y_tbias ;  //-
    startup_offset_y_encoder = ty - x_tbias ;
    
   //startup_offset_x_encoder = tx + y_tbias ;
   //startup_offset_y_encoder = ty - x_tbias ;  //+
    
    
    startup_offset_w_encoder =  _A *pi/float(180);    //degree 2 rad
    
    
}

void ActionEncoder_init()
{
    count=0;
    i=0;
    done=0;
    xangle=0;
    yangle=0;
    zangle=0;
    d_angle=0;
    pos_x=0;
    pos_y=0;
    angleSpeed=0;
    temp_zangle=0;
    LastRead=0;
    newDataArrived=false;

}


bool readEncoder(char c)
{
    //sprintf(buffer,"%02X",c);
    //sprintf(buffer,"%X",c);
    //pc.printf(buffer);
    //pc.printf("\r\n");
    
    //sprintf(buffer,"%d",count);
    //pc.printf(buffer);
    //pc.printf("\r\n");
    switch(count) {
        case 0:
            if (c==0x0d) count++;
            else count=0;
            break;
        case 1:
            if(c==0x0a) {
                i=0;
                count++;
            } else if(c==0x0d) {}
            else count=0;
            break;
        case 2:
            posture.data[i]=c;
            i++;
            if(i>=24) {
                i=0;
                count++;
            }
            break;
        case 3:
            if(c==0x0a)count++;
            else count=0;
            break;
        case 4:
            if(c==0x0d) {
                d_angle=posture.val[0]-temp_zangle;
                if (d_angle<-180) {
                    d_angle=d_angle+360;
                } else if (d_angle>180) {
                    d_angle=d_angle-360;
                }
                
                now_w+=d_angle;
                temp_zangle=posture.val[0];
                //xangle=posture.val[1];
                //yangle=posture.val[2];
                now_x=posture.val[3];
                now_y=posture.val[4];
                //angleSpeed=posture.val[5];
                newDataArrived=true;
                
            }
            count=0;
            done=1;
            LastRead=millis();
            return true;
            //break;
        default:
            count=0;
            break;
    }
    
    return false;
}

void inverse(float x_vel, float y_vel, float w_vel)
{
    motor1  =  int(   (    (-1) * sin(pi_div_3) * x_vel   +  cos(pi_div_3) * y_vel +  d * w_vel     )  * 60 / (wheelR * 2 * pi)  * gear   );
    motor2  =  int(   (    (-1) *  y_vel + d * w_vel                                                )  * 60 / (wheelR * 2 * pi)  * gear   ); 
    motor3  =  int(   (           sin(pi_div_3) * x_vel   +  cos(pi_div_3) * y_vel +  d * w_vel     )  * 60 / (wheelR * 2 * pi)  * gear   );
    
}



void motor_update()
{
    action1.SetVelocity_mod(motor1  * -1 );
    action2.SetVelocity_mod(motor2  * -1 );
    action3.SetVelocity_mod(motor3  * -1 );
    wait(0.005);
}

void odom_update()
{

    calculatePos(now_x,now_y,now_w);
     
    if ((    (fabs(target_x - transformed_real_now_x)) < tolerance_x ) && (   (fabs(target_y - transformed_real_now_y)) < tolerance_y )  && (   (fabs(target_w - transformed_real_now_w)) < tolerance_w )      )
    {
        point_counter+=1;

       tolerance_x = points[point_counter].required_tolerance_x;
       tolerance_y = points[point_counter].required_tolerance_x;
       tolerance_w = points[point_counter].required_tolerance_x;
       
       target_x    = points[point_counter].required_x                    ; //+    startup_offset_x_encoder;
       target_y    = points[point_counter].required_y                    ; //+    startup_offset_y_encoder;
       target_w    = points[point_counter].required_w  *pi/float(180);       ;//-    startup_offset_w_encoder;
       ///
       pid_on      = points[point_counter].pid_on;
       
       speed_max_x = points[point_counter].required_speed_max_x;
       speed_max_y = points[point_counter].required_speed_max_y;
       speed_max_w = points[point_counter].required_speed_max_w;
       
       inverse( 0    ,     0         ,     0   );
       return;
     
    }
    

    if (pid_on)
    {
        local_vel_x = (fabs(target_x - transformed_real_now_x) > tolerance_x ) ?   constrain(  (x_PID_P * (target_x - transformed_real_now_x)    ), -speed_max_x,    speed_max_x)  : 0 ;
        local_vel_y = (fabs(target_y - transformed_real_now_y) > tolerance_y ) ?   constrain(  (y_PID_P * (target_y - transformed_real_now_y)    ), -speed_max_y,    speed_max_y)  : 0 ;
        local_vel_w = (fabs(target_w - transformed_real_now_w) > tolerance_w ) ?   constrain(  (w_PID_P * (target_w - transformed_real_now_w)    ), -speed_max_w,    speed_max_w)  : 0 ;  
    }
    else
    {
        local_vel_x = (fabs(target_x - transformed_real_now_x) > tolerance_x ) ?   constrain(  (   copysign( speed_max_x , (target_x - transformed_real_now_x)  )   ), -speed_max_x,    speed_max_x)  : 0 ;
        local_vel_y = (fabs(target_y - transformed_real_now_y) > tolerance_y ) ?   constrain(  (   copysign( speed_max_y ,  (target_y - transformed_real_now_y)  )   ), -speed_max_y,    speed_max_y)  : 0 ;
        local_vel_w = (fabs(target_w - transformed_real_now_w) > tolerance_w ) ?   constrain(  (   copysign( speed_max_w ,  (target_w - transformed_real_now_w)  )  ), -speed_max_w,    speed_max_w)  : 0 ; 
    }
    global_vel_x = local_vel_x * cos( -transformed_real_now_w  )  -  local_vel_y * sin( -transformed_real_now_w ); 
    global_vel_y = local_vel_x * sin( -transformed_real_now_w  )  +  local_vel_y * cos( -transformed_real_now_w );  //local to global transformation   (angle only)
    
    /*
    pc.printf("X: ");
    sprintf(buffer, "%f", transformed_real_now_x);
    pc.printf(buffer);
    pc.printf("  Y: ");
    sprintf(buffer, "%f", transformed_real_now_y);
    pc.printf(buffer);
    pc.printf("  W: ");
    sprintf(buffer, "%f", transformed_real_now_w);
    pc.printf(buffer);
    
    pc.printf(" | Global: ");
    sprintf(buffer, "%f", global_vel_x);
    pc.printf(buffer);
    pc.printf("  ");
    sprintf(buffer, "%f", global_vel_y);
    pc.printf(buffer);
    pc.printf("  ");
    sprintf(buffer, "%f", local_vel_w);
    pc.printf(buffer);*/
    
    
  
    inverse( global_vel_x   ,  global_vel_y       ,     local_vel_w   );

}

int main() {
   
//TEST
    
    points[0] = (point_info){.required_x = 0      ,  .required_y =   0     ,    .required_w =  0,    .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true};  
    points[1] = (point_info){.required_x = 0      ,  .required_y =   0     ,    .required_w =  0,    .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true}; 
    points[2] = (point_info){.required_x = 3      ,  .required_y =   0     ,    .required_w =  0,   .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true}; 
    points[3] = (point_info){.required_x = 3      ,  .required_y =   0.2   ,    .required_w =  0,   .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true};
    points[4] = (point_info){.required_x = 3      ,  .required_y =   0     ,    .required_w =  0,    .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true};
    points[5] = (point_info){.required_x = 0      ,  .required_y =   0     ,    .required_w =  0,    .required_tolerance_x = 0.02, .required_tolerance_y = 0.02, .required_tolerance_w = 0.05, .required_speed_max_x = 2.5, .required_speed_max_y = 0.5, .required_speed_max_w = 1 ,.pid_on=true};
    
  
    Action.baud(115200);
    Action.format(8,SerialBase::None,1); 
    ActionEncoder_init();
    while(1) 
    {
        if (Action.readable())
        {
            char c = Action.getc();
            if (readEncoder(c))
            {
                //startup_offset_x_encoder = now_x/1000;
                //startup_offset_y_encoder = now_y/1000;
                //startup_offset_w_encoder = now_w/float(180)*pi;
                
                start_calculatePos(  (now_x),(now_y), now_w  );   //global
                break;
            
            }
            
        }
    }    //start first to take offset from encoder... in case already moved
    
 
   
    
    
    for( int a = 1; a < 2; a++ ){
      action1.Enable();
      action2.Enable();
      action3.Enable();
      wait(0.1);
      action1.SetOperationalMode();
      action2.SetOperationalMode();
      action3.SetOperationalMode();
      wait(0.1);
      action1.Configvelocity(100000, 100000);
      action2.Configvelocity(100000, 100000);
      action3.Configvelocity(100000, 100000);  
      wait(0.1);
   }
          
    motor_updater.attach(&motor_update, RATE);  
    //odom_updater.attach(&odom_update, RATE);
    
        
    while(1) 
    {
        if (Action.readable())
        {
            //pc.putc('a');
            char c = Action.getc();
            if(readEncoder(c)) odom_update();
        }
        
    }

       
}
