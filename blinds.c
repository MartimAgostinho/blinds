//Dependencies pigpio usbrelay packages
#include <stdlib.h>
#include <stdio.h>  //debug
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <pigpio.h>

#include "blinds.h"

#define CHARMAX 200
#define OUTFILENAME "OuTtmp.txt"
#define NUM_GPIOs 40
#define MsStr "MANUAL_SWITCH.txt"

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

void init_gpio_pins(/*home h*/){
    
    int log;
    char str_log[CHARMAX] = {0};
    char str[2] = {0};

    for(int i = 0; i < NUM_GPIOs;++i){

        log = gpioSetMode(i,PI_OUTPUT);

        if( log ){
            str[0] = i + '0'; 
            strapp(str_log,"Error setting gpio Mode on pin :",str,0);
            
            if(log == PI_BAD_GPIO){ 

                strcat(str_log," PI_BAD_GPIO"); 
            
            }else if (log == PI_BAD_MODE) {
                strcat(str_log," PI_BAD_MODE");
            }

            error_log(str_log,"start_blind" );
        }
        
        log = gpioWrite(i, PI_LOW);

        if( log ){
            str[0] = i + '0'; 
            strapp(str_log,"Error setting gpio Mode on pin :",str,0);
            
            if(log == PI_BAD_GPIO){ 

                strcat(str_log," PI_BAD_GPIO"); 
            
            }else if (log == PI_BAD_LEVEL) {
                strcat(str_log," PI_BAD_LEVEL");
            }

            error_log(str_log,"start_blind" );
        }
    }
}


void start_blind(/*home h*/){

    int log;
    
    log = gpioInitialise();

    //TODO use only pins in home

    if (log == PI_INIT_FAILED) {
        error_log("PI initialization failed", "start_blind");
    }

    init_gpio_pins();
}

void stop_blind(){

    gpioTerminate();

}

//initializes a blind
blind init_blind(char * room_name, char *ID, unsigned int port_open, unsigned int port_close){

    blind b = malloc(sizeof(struct blind_struct));
    char cmd[CHARMAX] = {0};

    strapp(cmd,"rm -f ",OUTFILENAME,0);//kinda desnecessario
    int log = system(cmd);
    
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
        strcpy(b->ID, ID);

        if( b->ID == NULL ){ malloc_error("blinds"); }
    }else{ 
        b->ID = 0;
    }
    
    b->port_open  = port_open;
    b->port_close = port_close;

    return b;
}

manual_switch init_manual_switch(char * ID,unsigned int port){

    return (manual_switch)init_blind(MsStr,ID,port,0);

}

void del_manual_switch(manual_switch ms){
    
    free(ms->room_name);
    if( ms->ID ){ free(ms->ID); }
    free(ms);
}

void del_blind(blind b){
    
    free(b->room_name);
    if( b->ID ){ free(b->ID); }
    free(b);
}

home init_home(){
    
    //memory allocation for home struct
    home h = malloc(sizeof(struct home_struct));
    if( h == NULL ){ malloc_error("init_home"); }
    h->n_blinds = 0;
    return h;
}

void add_ms_home(home h,manual_switch ms){
    h->ms = ms;
}

// adds a blind to a home
void add_blind(home h, blind b){

    //create copy and increase n_blinds
    blind * btmp = malloc( (sizeof(blind) * ++h->n_blinds));
    
    if( btmp == NULL ){ malloc_error("add_blind"); }

    //copy
    for(int i = 0 ; i < (h->n_blinds - 1); ++i){

        btmp[i] = h->home_blinds[i];
    }
    
    //add new
    btmp[h->n_blinds - 1] = b;
    free(h->home_blinds);
    h->home_blinds = btmp;
}

//frees home struct memory
void del_home(home h){

    for(int i = h->n_blinds;i < 0;--i){

        del_blind(h->home_blinds[i]);
    }
    free(h->home_blinds);
    del_manual_switch(h->ms);
    free(h);
}

blind get_blind(home h,char * room_name){

    for(int i = 0;i < h->n_blinds;++i){

        if( !strcmp(room_name, h->home_blinds[i]->room_name) ){
            return h->home_blinds[i];
        }
    }

    return NULL;
}

//Saves a blind struct to a file
void fwrite_blind(blind b){

    //creating file name
    char * fname = malloc( sizeof(char) * ( strlen(b->room_name) + 5 ) );
    
    if( fname == NULL ){ malloc_error("fwrite_blind"); }

    fname[0] = 0;
    strcpy(fname, b->room_name);
    strcat(fname, ".txt");

    //opening file
    FILE * f = fopen(fname, "w");

    if( f == NULL ){ error_log("Error opening file", "fwrite_blind"); }

    //Writing to file
    fprintf(f,"%s\n%s\n%u\n%u",
            b->room_name,
            b->ID,
            b->port_open,
            b->port_close);

    if( ferror(f) ){ error_log("Error writing to file", "fwrite_blind"); }

    free(fname);
    fclose(f);

}

//escreve em ficheiros todos os estores da instancia home
void fwrite_home(home h , char * foldername){

    //WORK ARROUND TENHO DE MUDAR

    char cmd[CHARMAX] = {0};

    strapp(cmd,"rm -f ", foldername,0); //easy way to remote execution  
    system(cmd);                   //Nao estou muito preocupado pq é um projeoto pra minha casa :^(

    cmd[0] = 0;
    strapp(cmd,"mkdir ",foldername,0);
    system(cmd);
    
    cmd[0] = 0;
    strapp(cmd,"cd ",foldername);
    system(cmd);

    //WORK ARROUND TENHO DE MUDAR

    fwrite_blind((blind) h->ms);

    for(int i = 0;i < h->n_blinds;++i){

        fwrite_blind(h->home_blinds[i]);
    }

    //WORK ARROUND TENHO DE MUDAR
    system("cd ..");

}

//cria uma estrutura blind igual a guardada no ficheiro
blind fread_blind(char * filename){

    printf("%s\n",filename);
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

    //if str == "(null)" ID = 0
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

home fread_home(char * foldername){

    char cmd[CHARMAX]  = {0};  //mais seguro e facil de usar
    char str[CHARMAX]  = {0}; //do que usar mallocs
    char path[CHARMAX] = {0};
    
    cmd[0] = 0;
    strapp(cmd,"ls --hide=",MsStr," ",foldername," > ",OUTFILENAME,0);
    system(cmd);

    FILE *fp = fopen(OUTFILENAME, "r");

    if( fp == NULL ){ error_log("Error opening file", "fread_home"); }

    home h = init_home();
    strapp(path,foldername,"/",MsStr,0);
    h->ms = fread_blind(path);
    while ( 1 ) {
        
        fscanf(fp, "%s",str);
        if( feof(fp) ){ break; }

        path[0] = 0;
        strapp(path,foldername,"/",str,0);
        add_blind(h,fread_blind(path));
    }
    
    fclose(fp);
    return h;
}

//state = 1, on
//state = 0, off
void set_switch_relay(manual_switch ms,char state){

    char cmd[CHARMAX]   = {0};//command 
    char tmp[10]        = {0};
    char nchar[2]       = {0};//port as string
    char schar[2]       = {0};//state as string

    //Log does not return a usefull value
    
    //Criar ficheiro com hidrawN
    cmd[0] = 0;
    strapp(cmd,"sudo dmesg | grep -i '",ms->ID,"' | tail -1 | grep -wo 'hidraw.[a-z]*' > ",OUTFILENAME,0);
    int log = system(cmd);
    
    //abrir ficheiro
    FILE *fp = fopen(OUTFILENAME, "r");

    //testa sucesso
    if( fp == NULL ){ error_log("Error opening file", "set_relay_blind"); }

    //le o conteudo
    fscanf(fp, "%s",tmp);

    //se nao houver nenhum hidraw na porta indicada
    if( tmp[1] == 0 ){ 
        
        cmd[0] = 0;
        strapp(cmd,"ID ",ms->ID," not found",0);
        error_log(cmd, "set_relay_blind");
    }

    fclose(fp);

    //remove o ficheiro
    cmd[0] = 0;
    strapp(cmd,"rm -f ",OUTFILENAME,0);
    log = system(cmd);

    nchar[0] = ms->port_open + '0';
    schar[0] = state + '0';
    cmd[0] = 0;
    
    strapp(cmd,"sudo usbrelay /dev/",tmp,"_",nchar,"=",schar,0);
    log = system(cmd);
}

void set_switch_gpio(manual_switch ms, char state){

    unsigned int gpio = ms->port_open;

    //liga/desliga o switch manual
    int log = gpioWrite(gpio, state);

    //Processar resultado da operacao
    if( !log ){
        
        if( log == PI_BAD_GPIO ){ error_log(" BAD GPIO", "set_gpio_blind"); }
        else if( log == PI_BAD_LEVEL ){ error_log("BAD LEVEL", "set_gpio_blind"); }
        exit(EXIT_FAILURE);
    }
}

void set_switch(manual_switch ms, char state){

    if(ms->ID){
        set_switch_relay(ms,state);
    }else{
        set_switch_gpio(ms, state);

    }
}

//open = 1 -> open blind
//open = 0 -> close
void set_relay_blind(blind b, char open){

    //escrever no log se nao correr bem
    //sudo dmesg | grep -i 'usb-0000:04:00.3-2' | tail -1 | grep -wo 'hidraw.[a-z]*'
    //e processar o output para ter o numero de /dev/hidraw
    
    unsigned int gpio;
    char cmd[CHARMAX]   = {0};
    char tmp[10]        = {0};
    char nchar[2]       = {0};

    strapp(cmd,"sudo dmesg | grep -i '",b->ID,"' | tail -1 | grep -wo 'hidraw.[a-z]*' > ",OUTFILENAME,0);
    int log = system(cmd);
    //TODO  

    FILE *fp = fopen(OUTFILENAME, "r");

    if( fp == NULL ){ error_log("Error opening file", "set_relay_blind"); }

    if( open ){
        gpio = b->port_open;
    }else{
        gpio = b->port_close;
    }

    nchar[0] = gpio + '0';
    nchar[1] = 0;

    fscanf(fp, "%s",tmp);

    if( tmp[1] == 0 ){ 
        
        cmd[0] = 0;
        strapp(cmd,"ID ",b->ID," not found",0);
        error_log(cmd, "set_relay_blind");
    }

    cmd[0] = 0;
    
    strapp(cmd,"sudo usbrelay /dev/",tmp,"_",nchar,"=1",0);
    log = system(cmd);
    sleep(BLIND_TIME);

    cmd[0] = 0;
    strapp(cmd,"sudo usbrelay /dev/",tmp,"_",nchar,"=0",0);
    log = system(cmd);
    printf("/------------Done------------/\n");
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

void open_blind(home h, char * room_name){

    set_switch(h->ms, 1);

    blind b = get_blind(h,room_name);

    if ( b == NULL ) { goto end_opbl; }

    if( b->ID ){
        set_relay_blind(b, 1 );
        goto end_opbl;
        return;
    }

    set_gpio_blind(b, 1);
    end_opbl:
    set_switch(h->ms, 1);

}

void close_blind(home h, char * room_name){

    set_switch(h->ms, 1);

    blind b = get_blind(h,room_name);

    if ( b == NULL ) { goto end_opbl; }

    if( b->ID ){
        set_relay_blind(b, 0 );
        goto end_opbl;
        return;
    }

    set_gpio_blind(b, 0);
    end_opbl:
    set_switch(h->ms, 1);
}

/*--------------DEBUGFN--------------*/

void print_blind(blind b){

    printf("Room Name: %s\n",b->room_name);
    printf("ID: %s\n",b->ID);
    printf("Port Open: %u\n",b->port_open);
    printf("Port Close: %u\n",b->port_close);
}

void print_home(home h){

    printf("Numero de estores: %d\n",h->n_blinds);
    for(int i = 0;i < (h->n_blinds );++i){
        
        printf("\nEstore numero %d\n",i);
        print_blind(h->home_blinds[i]);
    }
    print_blind(h->ms);
}