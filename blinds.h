#ifndef _BLIND_H
#define _BLIND_H

#define BLIND_TIME 120 //tempo em segundos para fechar/abrir estores
#define BLIND_LOG_FILE "BlindLog.txt"

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
blind get_blind(home h,char * room_name);

//FILES
void fwrite_blind(blind b);
void fwrite_home(home h , char * foldername);
blind fread_blind(char * filename);
home fread_home(char * foldername);

//actions
void init_gpio_pins(/*home h*/);
void start_blind(/*home h*/);
void open_blind(blind b);
void close_blind(blind b);

/*--------------DEBUGFN--------------*/

void print_blind(blind b);
void print_home(home h);

/*-----------STRUCTS----------*/

struct blind_struct{
    
    char    *    room_name;    //string with room name no **spaces allowed**
    char    *    ID;          //0 if raspberry, relay its usb ex.:("usb-0000:04:00.3-2") 
    unsigned int port_open;  //if raspberry pi,its gpio port, if relay, its number
    unsigned int port_close;//==
};

struct home_struct{

    unsigned int n_blinds;   //Number of blinds
    blind * home_blinds;    //Array with lenght "n_blinds" pointing to blinds
};

#endif
