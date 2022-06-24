#include "blinds.h"
#include <stdlib.h>
#include <string.h>

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
void /*char*/ open_blind(blind b){
    

    if( !strcmp(b.ID, GPIO) ){ open_blind_gpio(b); }  

    else{ open_blind_relay(b); }

    //return res;
}

void /*char*/ close_blind(blind b){
    

    if( !strcmp(b.ID, GPIO) ){ open_blind_gpio(b); }  

    else{ open_blind_relay(b); }

    //return res;
}

