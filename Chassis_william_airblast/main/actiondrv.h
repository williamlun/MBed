#include "mbed.h"

class actionDrv
{
public:    
    actionDrv(int _id);
    void SetOperationalMode();
    void Enable();
    void Configvelocity(int acc, int dec);
    void SetVelocity(int vel);
    void SetVelocity_mod(int vel);  //added
    void send(char* msg);
    void send_mod(char* msg);  //added
    void stop();
    int id, SDOid;
    
};
