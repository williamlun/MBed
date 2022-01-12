#include "robot.h"
#include "pinDefind.h"


//Serial  device(PA_2, PA_3); // UART0 tx,rx
//Serial  PS(PA_9, PA_10);  // UART1 tx, rx

//extern volatile uint16_t PC_rx_in; //真傳

// Interupt Routine to read in data from serial port

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&   USE PVM INSTEAD OF IPM TO GO STRAIGHT LINE

int main() 
{
    PS4_SERIAL_INIT();
    SERIAL_SETUP(115200);
    //PC_SEND("  \r\n");
    //PC_SEND("CANnucleo board #1\r\n");
    CANOpen_SETUP(1000000,0.1);
    wait(1.0);
    STOP_SYNC();
    
    SDO_CW(1, CLEAR_FAULT);
    SDO_CW(2, CLEAR_FAULT);
    SDO_CW(3, CLEAR_FAULT);
    
    SET_NMT(RESET_COMMU);
    wait(0.01);
    if(!PDO_INIT()){PC_SEND("PDO INITIALIZATION FAILURE\r\n");}  //PDO INITIALIZE FUNCTION SHOULD BE MODIFIED BY USER
    //while(!PDO_INIT());
    
    SDO_CW(1, SHUT_DOWN);
    SDO_CW(2, SHUT_DOWN);
    SDO_CW(3, SHUT_DOWN);
    SET_NMT(SET_OP);
    wait(0.01);
    
    //**********CONFIGURATION DONE! READY FOR MOTION**********
    
    START_SYNC(0.05);
    //TEST P-VELOCITY MODE
    
    SET_OP_MODE(1, PVM);
    SET_OP_MODE(2, PVM);  
    SET_OP_MODE(3, PVM);
    
    SET_MAX_P_VELOCITY(1, 5000);
    SET_MAX_P_VELOCITY(2, 5000);
    SET_MAX_P_VELOCITY(3, 5000);
    
    SET_P_ACCELERATION(1, 15600);
    SET_P_ACCELERATION(2, 15600);
    SET_P_ACCELERATION(3, 15600);
    
    SET_P_DECELERATION(1, 15600);
    SET_P_DECELERATION(2, 15600);
    SET_P_DECELERATION(3, 15600);
    
    SET_QUICK_STOP_DECELERATION(1, 10400);
    SET_QUICK_STOP_DECELERATION(2, 10400);
    SET_QUICK_STOP_DECELERATION(3, 10400);
    
    SET_MOTION_PROFILE_TYPE(1, SIN);
    SET_MOTION_PROFILE_TYPE(2, SIN);
    SET_MOTION_PROFILE_TYPE(3, SIN);
    //              **********WARNING: HERE THE VALUE FROM CURRENT MODE IS ALSO EFFECTIVE! RESET THEM TO DEFAULT IF YOU DON'T WANT A CURRENT LIMIT!!!**********
    SET_CONTINOUS_CURRENT_LIMIT(1, 10000);
    SET_CONTINOUS_CURRENT_LIMIT(2, 10000);
    SET_CONTINOUS_CURRENT_LIMIT(3, 10000);
    
    SET_MAX_CURRENT_MODE_SPEED(1, 25000);
    SET_MAX_CURRENT_MODE_SPEED(2, 25000);
    SET_MAX_CURRENT_MODE_SPEED(3, 25000);
    
    SET_THERMAL_TIME_CONSTANT_WINDING(1, 40);
    SET_THERMAL_TIME_CONSTANT_WINDING(2, 40);
    SET_THERMAL_TIME_CONSTANT_WINDING(3, 40);
    
    

    //////////////////////////////////////////////////////////////////
    
    SDO_CW(1, SHUT_DOWN); 
    SDO_CW(2, SHUT_DOWN);
    SDO_CW(3, SHUT_DOWN);
    
    SDO_CW(1, SWITCH_ON_ENABLE_OP);
    SDO_CW(2, SWITCH_ON_ENABLE_OP);
    SDO_CW(3, SWITCH_ON_ENABLE_OP);
    
    SET_TARGET_VELOCITY(1, 500);
    SET_TARGET_VELOCITY(2, 500);
    SET_TARGET_VELOCITY(3, 500);
    wait(0.002);
    RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
    RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
    RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
    
    wait(0.3);
    SET_TARGET_VELOCITY(1, 0);
    SET_TARGET_VELOCITY(2, 0);
    SET_TARGET_VELOCITY(3, 0);
    RPDO1_EXE(1, SWITCH_ON_ENABLE_OP);
    RPDO1_EXE(2, SWITCH_ON_ENABLE_OP);
    RPDO1_EXE(3, SWITCH_ON_ENABLE_OP);
    
    //wait(2);
    //SYNC_SEND();
    //PC_SEND("SYNC1 sent!\r\n");
    //wait(2);
    //SYNC_SEND();
    //PC_SEND("SYNC2 sent!\r\n");
   // wait(2);
    //SYNC_SEND();
    //PC_SEND("SYNC3 sent!\r\n");
    
   
    
    
    while(1){
        checkFlatLoop();
        //SEND();
        }
}
