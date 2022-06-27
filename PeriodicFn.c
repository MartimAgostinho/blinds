#include <stdlib.h>

#include "PeriodicFn.h"

linked_fn make_linked_fn(){

    linked_fn lkfn;

    lkfn.head = NULL;
    lkfn.tail = NULL;

    return lkfn;
}

void add_fn(linked_fn lkfn,void (* fn)(void **),void ** args,unsigned long int min){

    if( lkfn.head == NULL ){
        
        lkfn.head = ( periodic_node * )(malloc( sizeof(periodic_node) ));//TODO free's
        lkfn.head->head_fn = (fn_node *) malloc( sizeof(fn_node) );     //TODO free's
        lkfn.head->head_fn->args = args;
        lkfn.head->head_fn->fn_ptr = fn;
        lkfn.head->head_fn->next_fn = NULL;
        lkfn.tail = lkfn.head;
        return;

    }else if( lkfn.head == lkfn.tail && min != lkfn.head->time){

        return;
    }

    

}

void del_linked_fn(linked_fn lkfn){

}

void start_fn(linked_fn lkfn);


