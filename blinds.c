#include <stdlib.h>
#include <string.h>
#include <pigpio.h>
#include <unistd.h>

#include "blinds.h"


void malloc_error(char  * strinfo){

    //TODO: escrever num log file
    exit(EXIT_FAILURE);

}

blind init_blind(char * room_name, char *ID, unsigned int port_open, unsigned int port_close){

    blind b = malloc(sizeof(struct blind_struct));
    
    if( b == NULL ){ malloc_error("blinds"); }

    b->room_name = (char *)malloc( sizeof(char) * strlen(room_name));

    if( b->room_name == NULL ){ malloc_error("blinds"); }

    if( ID ){ 
        b->ID = (char *)malloc( sizeof(char) * strlen(ID));
        if( b->ID == NULL ){ malloc_error("blinds"); }
    }else{ 
        b->ID = 0;
    }
    
    b->port_open = port_open;
    b->port_close = port_close;

    return b;
}

void del_blind(blind b){
    
    free(b->room_name);
    if( b->ID ){ free(b->ID); }
    free(b);
}

home init_home(){
    
    //memory allocation for home struct
    home h = malloc(sizeof(struct home_struct));
    if( h == NULL ){ malloc_error("Creating Home"); }
    return h;
}

// adds a blind to a home
void add_blind(home h, blind b){

    //create copy and increase n_blinds
    blind * btmp = malloc( (sizeof(blind) * ++h->n_blinds));
    
    if( btmp == NULL ){ malloc_error("adding a blind "); }

    //copy
    for(int i = 0 ; i < (h->n_blinds - 1); ++i){

        btmp[i] = h->home_blinds[i];
    }
    
    //add new
    btmp[h->n_blinds] = b;
    free(h->home_blinds);
    h->home_blinds = btmp;
}

//frees home struct memory
void del_home(home h){

    for(int i = h->n_blinds;i < 0;--i){

        del_blind(h->home_blinds[i]);
    }
    free(h->home_blinds);
    free(h);
}

//open = 1 -> open blind
//open = 0 -> close
void set_relay_blind(blind b, char open){
    //ver se esta inicializado 
    //set rele
    //escrever no log se nao correr bem
}

//open = 1 -> open blind
//open = 0 -> close
void set_gpio_blind(blind b, char open){
    
    //ver se esta inicializado 
    //set gpio
    //escrever no log se nao correr bem
    unsigned int gpio;

    if( open ){
        gpio = b->port_open;
    }else{
        gpio = b->port_close;
    }

    int log = gpioWrite(gpio, 1);

    if( !log ){
        //TODO: save to a file
        if( log == PI_BAD_GPIO ){ printf(" BAD GPIO\n "); }
        else if( log == PI_BAD_LEVEL ){ printf(" BAD LEVEL\n "); }
        exit(EXIT_FAILURE);
    }

    sleep(BLIND_TIME);
}

void open_blind(blind b){

    if( b->ID ){
        set_relay_blind(b, 1 );
        return;
    }

    set_gpio_blind(b, 1);
}

void close_blind(blind b){

    if( b->ID ){
        set_relay_blind(b, 0 );
        return;
    }

    set_gpio_blind(b, 0);
}
