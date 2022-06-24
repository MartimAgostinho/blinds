#ifndef _BLIND_H_
#define _BLIND_H_

#define GPIO "gpio"
#define BLIND_TIME 100 //time in seconds

typedef struct {

    char *  room;
    char *  ID;
    int  *  open_hours;
    int  *  close_hours;
    int     port_open;
    int     port_close;

}blind;

void open_blind(blind b);

#endif
