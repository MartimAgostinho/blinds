#ifndef _BLIND_H
#define _BLIND_H

#define BLIND_TIME 120 //tempo em segundos para fechar/abrir estores

typedef struct blind_struct * blind;
typedef struct home_struct * home;


blind init_blind(char * room_name,
        char * ID,
        unsigned int port_open,
        unsigned int port_close);
void del_blind(blind b);
home init_home();
void add_blind(home h,blind b);
void del_home(home h);
void open_blind(blind b);
void close_blind(blind b);


/*-----------STRUCTS----------*/
struct blind_struct{
    
    char    *    room_name;        //string with room name
    char    *    ID;              //0 if raspberry, relay its id ex.:("/dev/hidraw10") 
    unsigned int port_open;    //if raspberry pi,its gpio port, if relay, its number
    unsigned int port_close;  //==
};

struct home_struct{

    unsigned int n_blinds;   //Number of blinds
    blind * home_blinds;    //Array with lenght "n_blinds" pointing to blinds
};

#endif
