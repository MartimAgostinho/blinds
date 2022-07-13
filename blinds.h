#ifndef _BLIND_H_
#define _BLIND_H_

#define GPIO "gpio"
#define BLIND_TIME 100 //time in seconds

typedef struct {

    char *  room_name;        //string with room name
    char *  ID;              //0 if raspberry, relay its id ex.:("/dev/hidraw10") 
    int  *  open_hours;     //list with opening hours in minutes, since midnight
    int  *  close_hours;   //==
    int     port_open;    //if raspberry pi,its gpio port, if relay, its number
    int     port_close;  //==

}blind;

void open_blind(blind b);

#endif
