#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pigpio.h>
#include <unistd.h>
#include <time.h>

#include "blinds.h"

#define CHARMAX 200

void error_log(char  * strinfo,char * fn_name){

    FILE * f = fopen(BLIND_LOG_FILE, "a");
    time_t current_time = time(NULL);
    char * time_string = ctime(&current_time);
    
    if( f == NULL ){ 
        printf("ERROR OPENING FILE %s\n",BLIND_LOG_FILE);
        exit(EXIT_FAILURE);    
    }

    fprintf(f, "-------------ERROR-------------\n");
    fprintf(f, "Error message: %s\n",strinfo);
    fprintf(f, "Function: %s\n",fn_name);
    fprintf(f, "Date and Time: %s\n",time_string);

    fclose(f);
    exit(EXIT_FAILURE);
}

void malloc_error(char * fn_name){

    error_log("Error allocating memory",fn_name);

}

//initializes a blind
blind init_blind(char * room_name, char *ID, unsigned int port_open, unsigned int port_close){

    blind b = malloc(sizeof(struct blind_struct));
    
    if( b == NULL ){ malloc_error("blinds"); }

    if( !(room_name == NULL )){ 
        b->room_name = (char *)malloc( sizeof(char) * strlen(room_name));
        strcpy(b->room_name, room_name);

        if( b->room_name == NULL ){ malloc_error("blinds"); }
    
    }else{ 
    
        b->room_name = 0;
    }
    
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

//Saves a blind struct to a file
void fwrite_blind(blind b){

    //creating file name
    char * fname = malloc( sizeof(char) * ( strlen(b->room_name) + 4 ) );
    
    if( fname == NULL ){ malloc_error("fwrite_blind"); }

    fname[0] = 0;
    strcpy(fname, b->room_name);
    strcat(fname, ".txt");

    //opening file
    FILE * f = fopen(fname, "w");

    if( f == NULL ){ error_log("Error opening file", "fwrite_blind"); }

    //Writing to file
    fprintf(f,"%s\n%s\n%u\n%u",b->room_name,b->ID,b->port_open,b->port_close);

    if( ferror(f) ){ error_log("Error writing to file", "fwrite_blind"); }

    fclose(f);

}

//escreve em ficheiros todos os estores da instancia home
void fwrite_home(home h){

    for(int i = 0;i < h->n_blinds;++i){

        fwrite_blind(h->home_blinds[i]);
    }
}

//cria uma estrutura blind igual a guardada no ficheiro
blind fread_blind(char * filename){

    //init blind
    blind b = init_blind(NULL, 0 , 0 , 0);

    if( b == NULL ){ error_log("Creating a struct blind", "fread_blind"); }

    char * strtmp = malloc(sizeof(char) * CHARMAX);

    //opening file
    FILE * f = fopen(filename, "r");

    if( f == NULL ){ error_log("Error opening file", "fread_blind"); }

    fscanf(f,"%s",strtmp);

    b->room_name = (char *)malloc(sizeof(char) * strlen(strtmp));

    if( b->room_name == NULL ){ malloc_error("fread_blind"); }
    strcpy(b->room_name, strtmp);

    strtmp[0] = 0;
    fscanf(f,"%s",strtmp);

    //if 
    if( strcmp(strtmp, "(null)") ){
    
        b->ID = (char *)malloc(sizeof(char) * strlen(strtmp));

        if( b->ID == NULL ){ malloc_error("fread_blind"); }

        strcpy(b->ID, strtmp);
    }

    fscanf(f,"%u\n%u",&b->port_open,&b->port_close);

    if( ferror(f) ){ error_log("Error reading file", "fread_blind"); }

    free(strtmp);
    return b;
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
    unsigned int gpio;

    if( open ){
        gpio = b->port_open;
    }else{
        gpio = b->port_close;
    }

    //comeca a abrir ou fechar os estores
    int log = gpioWrite(gpio, 1);

    //Processar resultado da operacao
    if( !log ){
        
        if( log == PI_BAD_GPIO ){ error_log(" BAD GPIO", "set_gpio_blind"); }
        else if( log == PI_BAD_LEVEL ){ error_log("BAD LEVEL", "set_gpio_blind"); }
        exit(EXIT_FAILURE);
    }

    sleep(BLIND_TIME);

    log = gpioWrite(gpio, 0);

    //Processar resultado da operacao
    if( !log ){
        
        if( log == PI_BAD_GPIO ){ error_log(" BAD GPIO", "set_gpio_blind"); }
        else if( log == PI_BAD_LEVEL ){ error_log("BAD LEVEL", "set_gpio_blind"); }
        exit(EXIT_FAILURE);
    }

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
/*--------------DEBUGFN--------------*/

void print_blind(blind b){

    printf("Room Name: %s\n",b->room_name);
    printf("ID: %s\n",b->ID);
    printf("Port Open: %u\n",b->port_open);
    printf("Port Close: %u\n",b->port_close);
}

void print_home(home h){

    for(int i = 0;i < h->n_blinds;++i){

        print_blind(h->home_blinds[i]);
        printf("\nEstore numero %d\n",i);
    }
}