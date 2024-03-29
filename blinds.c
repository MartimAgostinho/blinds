#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "blinds.h"
#include "sequence.h"

#define MAXCHAR 250

//NEEDS SUDO PERMISSION
void strapp(char *str,...);
void wait(int seconds);
void intcpy( int * dest, int * src );

void intcpy( int * dest, int * src ){

    for(int i = 0;i < src[0];++i){ dest[i] = src[i]; }

}

blind make_blind(char * room_name,char * ID,int * open_hours,int * close_hours,unsigned int port_open,unsigned int port_close ){

    blind b;
    
    b.room_name = (char *)malloc( sizeof(char) * ( strlen(room_name) + 1) );
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

    if( !set && set != 1 ){ 
        printf("set can only be 1 or 0\nset value = %d\nexiting...\n",set);
        exit(1);
    }

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

void set_blind(blind b,char open,char set){
    
    unsigned int port;
    //defining if im closing or opening the blinds 
    if( open ){ port = b.port_open; }
    else{ port = b.port_close; }

    if( !b.ID ){//GPIO 
        set_blind_gpio(port,set);
    }  

    else{ 
        set_blind_relay(port,b.ID,set);
    }

}

void  open_blind(blind b){

   set_blind(b,1,1);
   wait(BLIND_TIME);
   set_blind(b,1,0);
}

//TODO: use list
void close_blind(blind b){

   set_blind(b,0,1);
   wait(BLIND_TIME);
   set_blind(b,0,0);
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

/*      Files       */

//Reads a single file 
blind fread_blind(FILE *fp/*const char * filename*/){//TODO: make this return a pointer for obvious reasons

    if( fp == NULL ){ exit(1); }

    char         room_name[MAXCHAR];  
    char         ID[MAXCHAR];         
    int     *    open_hours;     
    int     *    close_hours;
    unsigned int port_open;  
    unsigned int port_close;
    int i,j;

    fscanf(fp," %[^\n]", room_name);
    fscanf(fp," %[^\n]", ID);

    fscanf(fp,"%d",&j);
    open_hours    = (int *)malloc( sizeof(int) * j );
    open_hours[0] = j;    

    for(i = 1; i < j;++i){ fscanf(fp,"%d",&(open_hours[i])); }

    fscanf(fp,"%d",&j);
    close_hours    = (int *)malloc( sizeof(int) * j );
    close_hours[0] = j;    

    for(i = 1; i < j;++i){ fscanf(fp,"%d",&close_hours[i]);}

    fscanf(fp,"%u",&port_open);
    fscanf(fp,"%u",&port_close);

    blind b = make_blind(room_name, ID ,
            open_hours ,
            close_hours,
            port_open  ,
            port_close );
    
    return b;
}

//for multiple blinds 
sequence Fload_blind(const char * filename){
    
    FILE *fp = fopen(filename, "r");
    unsigned int n;
    blind *b;

    fscanf(fp, "%u" ,&n);
    sequence s = make_sequence(n);

    if( fp == NULL ){
        //handle error
        fclose(fp);
        exit(0);
    }
    for(int i = 0;i < n;++i){ 
        b = malloc(sizeof(blind));
        *b = fread_blind(fp);
        add_elem(&s, b);
    }
    fclose(fp);
    return s;
}

/*      Files       */

//debug
void print_blind(blind b){

    int i;
    printf("\nPRINT BLIND\n\n");
    printf("Room name:%s\n",b.room_name);
    if( !b.ID ){ printf("ID: 0\n"); }
    else{ printf("ID: %s\n",b.ID); }
    
    printf("open hours:");
    for(i = 1;i < b.open_hours[0];++i){ printf("%d,",b.open_hours[i]); }
    printf("\nclose hours:");
    for(i = 1;i < b.close_hours[0];++i){ printf("%d,",b.open_hours[i]); }
    
    printf("\nOpening port:%u\n",b.port_open);
    printf("Closing port:%u\n",b.port_close);
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
