#include <stdlib.h>

#include "PeriodicFn.h"

linked_fn make_linked_fn(){

    linked_fn lkfn;

    lkfn.head = NULL;
    lkfn.tail = NULL;

    return lkfn;
}

void add_fn(linked_fn lkfn,void (* fn)(void **),void ** args,unsigned long int min){


    fn_node * fn_new = (fn_node * ) malloc( sizeof(fn_node) );
    
    if( fn_new == NULL ){ return; }

    fn_new->next_fn  = NULL;
    fn_new->fn_ptr   = fn;
    fn_new->args     = args;

    if( lkfn.head == NULL ){
        
        lkfn.head = ( periodic_node * )(malloc( sizeof(periodic_node) ));//TODO free's
        
        if( lkfn.head == NULL ){ return; }
        
        lkfn.head->head_fn  = fn_new;
        lkfn.head->next     = lkfn.head;
        lkfn.tail           = lkfn.head;
        
        return;

    }else if( lkfn.head == lkfn.tail ){
        
                
        if( min == lkfn.head->time ){//just for safety probs useless
            
            fn_node fn_aux = *lkfn.head->head_fn;

            while( fn_aux.next_fn != NULL ){//get to last node
                fn_aux      = *fn_aux.next_fn;
                
            }fn_aux.next_fn = &fn_aux;

        }else{

            periodic_node *prn_aux = ( periodic_node * )malloc( sizeof(periodic_node) );

            if( prn_aux == NULL ){ return; }
            
            prn_aux->head_fn  = fn_new;
            
            if( min > lkfn.head->time ){
                
                prn_aux->time   = min - lkfn.head->time;
                lkfn.tail       = prn_aux;
                lkfn.head->next = prn_aux;   

            }else{//TODO verify
                
                prn_aux->time   = min;
                lkfn.head       = prn_aux;
                lkfn.head->next = lkfn.tail;
                lkfn.tail->next = prn_aux;
                lkfn.tail->time -= min;   
            }
        }

        return;
    }

    

}

void del_linked_fn(linked_fn lkfn){

}

void start_fn(linked_fn lkfn);


