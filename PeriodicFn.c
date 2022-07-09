#include <stdio.h>
#include <stdlib.h>

#include "PeriodicFn.h"

linked_fn make_linked_fn(){

    linked_fn lkfn;

    lkfn.head = NULL;
    lkfn.tail = NULL;

    return lkfn;
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

void add_fn(linked_fn *lkfn,void (* fn)(void **),void ** args,unsigned long int min,unsigned int id){


    fn_node * fn_new = (fn_node * ) malloc( sizeof(fn_node) );
    
    if ( fn_new == NULL ){ return; }

    fn_new->next_fn  = NULL;
    fn_new->fn_ptr   = fn;
    fn_new->args     = args;
    fn_new->id       = id;

    if ( lkfn->head == NULL ){
         //first node
        //null node just with time

        lkfn->head = ( periodic_node * )(malloc( sizeof(periodic_node) ));
        lkfn->tail = ( periodic_node * )(malloc( sizeof(periodic_node) ));

        if ( lkfn->head == NULL ){ return; }
        
        lkfn->head->head_fn  = fn_new;
        lkfn->head->time     = min;
        lkfn->tail->time     = DAYMIN - min; //time till midnight
        lkfn->tail->head_fn  = 0;           //NULL node
        lkfn->tail->next     = lkfn->head; //loop
        lkfn->head->next     = lkfn->tail;//loop
        
        return;
    }

    periodic_node * prn_aux = lkfn->head, * prn_prev = lkfn->tail;
    unsigned long int min_aux = prn_aux->time;

    if ( min >= min_aux ){//if its not the head

        while (min > min_aux){//get the relative time, prev node and nxt node
            
            min      -= min_aux;
            prn_prev =  prn_aux;
            prn_aux  =  prn_aux->next;
            min_aux  =  prn_aux->time;
        }

        if ( min == min_aux ){// if node already exists 
            
            fn_node * fn_aux = prn_aux->head_fn;
            prn_aux->head_fn = fn_new;
            fn_new->next_fn  = fn_aux;
            return;
        }
        periodic_node * prn_new = ( periodic_node * )malloc(sizeof(periodic_node));
        if ( prn_new == NULL ){ return; }
        
        //create new node 
        prn_new->head_fn = fn_new;
        prn_new->next    = prn_aux;
        prn_new->time    = min;
        prn_prev->next   = prn_new;

        do{//update relative times
            prn_aux->time -= min;
            prn_aux       =  prn_aux->next;

        }while( prn_aux != lkfn->tail );    
        return;
    }
     //TODO fix redundant 
    //if time is less than head
    periodic_node * prn_new = ( periodic_node * )malloc(sizeof(periodic_node));
    if ( prn_new == NULL ){ return; }

    //create new node 
    prn_new->head_fn = fn_new;
    prn_new->next    = prn_aux;
    prn_new->time    = min;
    prn_prev->next   = prn_new;
    
    do{//update relative times
        prn_aux->time -= min;
        prn_aux       =  prn_aux->next;

    }while( prn_aux != lkfn->tail );
    lkfn->head = prn_new;//update head
}

void rm_fn(linked_fn * lkfn,unsigned int id){

    periodic_node * prn_aux  = lkfn->head;
    periodic_node * prn_prev = lkfn->tail;
    fn_node       * fn_prev = NULL;
    fn_node       * fn_aux;

    while (prn_aux != lkfn->tail) {
        
        fn_aux = prn_aux->head_fn;
        
        while (fn_aux != NULL) {
            
            if ( fn_aux->id == id ) {
                
                if (fn_prev == NULL) {//if its de only fn on the node
                    
                    unsigned long int min = prn_aux->time;
                    free(fn_aux);

                    if ( prn_prev == lkfn->tail) {
                        lkfn->tail->next = prn_aux->next;//next from head node
                        lkfn->head       = lkfn->tail->next;
                    
                    }else{
                        prn_prev->next = prn_aux->next;
                    }
                    free( prn_aux );
                    
                    do{
                        prn_aux->time += min;
                        prn_aux = prn_aux->next;
                    }while( prn_aux != lkfn->tail );
                    return;
                }
                free(fn_aux);

                //TODO if its the last node or not
            }
            fn_prev = fn_aux;
            fn_aux  = fn_aux->next_fn;
        }
        prn_prev = prn_aux;
        prn_aux  = prn_aux->next;
    }

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
