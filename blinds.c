#include <pigpio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "blinds.h"

//NEEDS SUDO PERMISSION
void strapp(char *str,...);
void wait(int seconds);
void intcpy( int * dest, int * src );

void intcpy( int * dest, int * src ){

    for(int i = 0;i < src[0];++i){ dest[0] = src[0]; }

}

blind make_blind(char * room_name,char * ID,int * open_hours,int * close_hours,unsigned int port_open,unsigned int port_close ){

    blind b;
    
    b.room_name = (char *)malloc( sizeof(char) * ( strlen(room_name + 1) ) );
    if( b.room_name == NULL ){ exit(0); }
    strcpy(b.room_name, room_name);

    if( !ID ){ b.ID = 0; }
    else{
        b.ID = (char *)malloc( sizeof(char) * ( strlen(ID) + 1));
        if( b.ID == NULL ){ exit(0); }
        strcpy(b.ID, ID);
    }
    
    b.open_hours = (int *)malloc( sizeof(int) * (open_hours[0]) );
    if(b.open_hours == NULL){ exit(0); }
    intcpy(b.open_hours, open_hours);

    b.close_hours = (int *)malloc( sizeof(int) * (close_hours[0]) );
    if(b.close_hours == NULL){ exit(0); }
    intcpy(b.close_hours, close_hours);

    b.port_open  = port_open;
    b.port_close = port_close;

    return b;
}

void free_blind(blind b){
    free(b.room_name);
    free(b.ID);
    free(b.open_hours);
    free(b.close_hours);
}

void set_blind_gpio(unsigned int ID,int set){
    
    int log = gpioWrite(ID, set);

    if( !log ){
        //TODO: save to a file
        if( log == PI_BAD_GPIO ){ printf(" BAD GPIO\n "); }
        else if( log == PI_BAD_LEVEL ){ printf(" BAD LEVEL\n "); }
        exit(0);
    }

}

void set_blind_relay( int relay,char * id,char set ){

    char * cmd = (char *)malloc( sizeof(char) * ( strlen(id) + 14 ) );//14 = number of char in "usbrelay ID_n=s"
    
    if( cmd == NULL ) { 
        free(cmd);
        exit(0);
    }
    
    char rel[2],st[2];
    
    rel[0] = relay +'0';//conversion to char
    rel[1] = '\0';
    st [0] = set +'0';
    st [1] = '\0';

    cmd[0] = '\0';
    //build shell comand
    strapp(cmd,"usbrelay ",id,"_",rel,"=",st,0);    
    int log = system(cmd);
    
    if( !log ){//TODO: save log to file
        free(cmd);
        exit(0);
    }

    free(cmd);
}

void  open_blind(blind b){

    if( !b.ID ){//GPIO 
        set_blind_gpio(b.port_open,1);
        wait(BLIND_TIME);
        set_blind_gpio(b.port_open,0);
    }  

    else{ 
        set_blind_relay(b.port_open,b.ID,1);
        wait(BLIND_TIME);
        set_blind_relay(b.port_open,b.ID,0);
    }
}

void close_blind(blind b){

    if( !b.ID ){ 
        set_blind_gpio(b.port_close,1);
        wait(BLIND_TIME);
        set_blind_gpio(b.port_close,0);
    }  

    else{ 
        set_blind_relay(b.port_close,b.ID,1);
        wait(BLIND_TIME);
        set_blind_relay(b.port_close,b.ID,0);
    }
}

void open_blind_gen( void ** args ){
    
    if( !args[1] ){ return; }//args have to end with value 0( prevent coredump )

    blind *b = args[0];

    open_blind( *b );
}

void close_blind_gen( void ** args ){
    
    if( !args[1] ){ return; }//args have to end with value 0( prevent coredump )

    blind *b = args[0];

    close_blind( *b );
}

void wait(int seconds){

}

//str needs to have enough memory allocated
//args have to end with a 0
void strapp(char *str,...){
    
    va_list args;
    va_start(args,str);
    
    char * str2 = va_arg(args, char *);

    while( str2 != 0 ){

        strcat(str, str2);
        str2 = va_arg(args, char *);
    }
    va_end(args);
}
