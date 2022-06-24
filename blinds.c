#include "blinds.h"
#include <stdlib.h>
#include <string.h>

void wait(int seconds);

void open_blind_gpio(blind b){
    //Abrir estores pelo GPIO do raspberry pi
}

void open_blind_relay(blind b){
    //usar o usbrelay
}

void close_blind_gpio(blind b){
    //Abrir estores pelo GPIO do raspberry pi
}

void close_blind_relay(blind b){
    //usar o usbrelay
}

void stop_blind_gpio(blind b){

}

void stop_blind_relay(blind b){

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

void /*char*/ close_blind(blind b){
    

    if( !strcmp(b.ID, GPIO) ){ 
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

