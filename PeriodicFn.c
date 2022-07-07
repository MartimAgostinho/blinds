#include <stdio.h>
#include <stdlib.h>

#include "PeriodicFn.h"

linked_fn make_linked_fn(){

    linked_fn lkfn;

    lkfn.head = NULL;
    lkfn.tail = NULL;

    return lkfn;
}

void add_fn(linked_fn *lkfn,void (* fn)(void **),void ** args,unsigned long int min){


    fn_node * fn_new = (fn_node * ) malloc( sizeof(fn_node) );
    
    if( fn_new == NULL ){ return; }

    fn_new->next_fn  = NULL;
    fn_new->fn_ptr   = fn;
    fn_new->args     = args;

    if( lkfn->head == NULL ){//TODO redoo tail cant be == to head
        
         //first node
        //null node just with time

        lkfn->head = ( periodic_node * )(malloc( sizeof(periodic_node) ));//TODO free's
        lkfn->tail = ( periodic_node * )(malloc( sizeof(periodic_node) ));

        if( lkfn->head == NULL ){ return; }
        
        lkfn->head->head_fn  = fn_new;
        lkfn->head->time     = min;
        //printf("TIME!:%d\n",lkfn->head->time);
        lkfn->tail->time     = DAYMIN - min;//time till midnight
        lkfn->tail->head_fn  = NULL;       //dummy node
        lkfn->tail->next     = lkfn->head; //loop
        lkfn->head->next     = lkfn->tail;//loop
        
        return;

    }else if( lkfn->tail->head_fn == NULL ){
        
        if( min == lkfn->head->time ){//just for safety probs useless
            
            fn_new->next_fn    = lkfn->head->head_fn;
            lkfn->head->head_fn = fn_new; 
            return;
        
        }/*else{

            periodic_node *prn_aux = ( periodic_node * )malloc( sizeof(periodic_node) );

            if( prn_aux == NULL ){ return; }
            
            prn_aux->head_fn  = fn_new;
            
            if( min > lkfn.head->time ){
                
                prn_aux->time   = min - lkfn.head->time;
                lkfn.tail       = prn_aux;
                lkfn.head->next = prn_aux;   

            }else{ 

                prn_aux->time   = min;
                prn_aux->next   = lkfn.tail;
                lkfn.head       = prn_aux;
                lkfn.tail->next = lkfn.head;
                lkfn.tail->time -= min;   
            }
        }*/
    }

    //1 find linked_fn place
    
    periodic_node *prn_new;
    periodic_node *prn_aux = lkfn->head, *prn_prev = lkfn->tail;
    unsigned long int min_aux = prn_aux->time;
    
    while( min_aux >= min ){

        min      -= min_aux;
        prn_prev =  prn_aux;
        prn_aux  =  prn_aux->next;
        min_aux  =  prn_aux->time;
    }
    //printf("min%lu\nmin_aux:%lu\nprn_prev:%p\nprn_aux:%p\n",min,min_aux,prn_prev,prn_aux);
    
    if( min_aux == min ){
        
        fn_node *fn_aux = prn_aux->head_fn;
        //prn_new = prn_aux;
        
        //while( fn_aux != NULL ){ fn_aux = fn_aux->next_fn; }
        prn_aux->head_fn = fn_new;
        fn_new->next_fn  = fn_aux;
    }

    else{

        prn_new          = (periodic_node *)malloc( sizeof(periodic_node) );
        prn_new->head_fn = fn_new;
        prn_new->time    = min;
        prn_new->next    = prn_aux;
        prn_prev->next   = prn_new;

        while( prn_aux != lkfn->head ){

            prn_aux->time -= min;
            prn_aux       =  prn_aux->next;
        }

    }

    //2 find periodic_node place 
    //place it
    //change  next nodes time
    //success!
}


void del_linked_fn(linked_fn lkfn){

    periodic_node * prn_aux = lkfn.head;
    periodic_node * prn_nxt = NULL;
    fn_node       * fn_aux = prn_aux->head_fn;
    fn_node       * fn_nxt;
    
    do{
        
        //delete fn_nodes
        while( fn_aux != NULL ){

            fn_nxt = fn_aux->next_fn;
            free(fn_aux);
            fn_aux = fn_nxt;
        }
        
        prn_nxt = prn_aux->next;
        free(prn_aux);
        prn_aux = prn_nxt;

    }while( prn_aux != lkfn.head );
}

void exec_node(periodic_node *node){//use threads in the future 

    fn_node *fn_aux = node->head_fn;

    while( fn_aux != NULL ){

        fn_aux->fn_ptr(fn_aux->args);
        fn_aux = fn_aux->next_fn;
    }

}

void start_fn(linked_fn lkfn);

periodic_node * create_periodic_node(periodic_node * next_prn){

    periodic_node * prn = (periodic_node *)malloc( sizeof(periodic_node) );

    prn->head_fn    = NULL;
    prn->next       = next_prn;
    prn->time       = 0;
    
    return prn;

}
