#ifndef _BLIND_H_
#define _BLIND_H_

#include <pigpio.h>

#define GPIO "gpio"
#define BLIND_TIME 100 //time in seconds

typedef struct {

    char    *    room_name;        //string with room name
    char    *    ID;              //0 if raspberry, relay its id ex.:("/dev/hidraw10") 
    int     *    open_hours;     //First number is lenght, list with opening hours in minutes, since midnight
    int     *    close_hours;   //==
    unsigned int port_open;    //if raspberry pi,its gpio port, if relay, its number
    unsigned int port_close;  //==

}blind;

blind make_blind(char * room_name,char * ID,int * open_hours,int * close_hours,unsigned int port_open,unsigned int port_close );
void free_blind(blind b);
void open_blind(blind b);
void close_blind(blind b);
void open_blind_gen(void ** args);
void close_blind_gen(void ** args);

#endif
