#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "blinds.h"

//NEEDS SUDO PERMISSION
void strapp(char *str,...);
void wait(int seconds);

void set_blind_gpio(/*args*/){
    //Abrir estores pelo GPIO do raspberry pi
}

void set_blind_relay( int relay,char * id,char set ){

    char * cmd = (char *)malloc( sizeof(char) * ( strlen(id) + 14 ) );//14 = number of char in usbrelay ID_n=s
    char rel[2],st[2];
    
    rel[0] = relay +'0';
    rel[1] = '\0';
    st[0] = set +'0';
    st[1] = '\0';

    cmd[0] = '\0';
    //build shell comand
    strapp(cmd,"usbrelay ",id,"_",rel,"=",st,0);    
    system(cmd);
}

void open_blind_gpio(blind b){
    //Abrir estores pelo GPIO do raspberry pi
}

void open_blind_relay(blind b){
    set_blind_relay(b.port_open, b.ID ,1);
}

void close_blind_gpio(blind b){
    //Abrir estores pelo GPIO do raspberry pi
}

void close_blind_relay(blind b){
    set_blind_relay(b.port_close, b.ID ,1);
}

void stop_blind_gpio(blind b){

}

void stop_blind_relay(blind b){
    set_blind_relay(b.port_close, b.ID ,0);
    set_blind_relay(b.port_open, b.ID ,0);
}

void /*char*/ open_blind(blind b){
    

    if( !b.ID ){ 
        open_blind_gpio(b);
        wait(BLIND_TIME);
        stop_blind_gpio(b);
    }  

    else{ 
        open_blind_relay(b);
        wait(BLIND_TIME);
        stop_blind_gpio(b);
    }

    //return res;
}

void /*char*/ open_blind_gen( void ** args ){
    
    if( !args[1] ){ return; }//args have to end with value 0( prevent coredump )

    blind *b = args[0];

    open_blind( *b );

}

void /*char*/ close_blind(blind b){
    

    if( !b.ID ){ 
        close_blind_gpio(b);
        wait(BLIND_TIME);
        stop_blind_gpio(b);
    }  

    else{ 
        close_blind_relay(b);
        wait(BLIND_TIME);
        stop_blind_gpio(b);
    }

    //return res;
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
