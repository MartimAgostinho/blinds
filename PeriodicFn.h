#ifndef _PeriodicFn_H_
#define _PeriodicFn_H_

#define DAYMIN 1440

//********************structs********************//

typedef struct fn_node fn_node ;
typedef struct periodic_node periodic_node;

struct fn_node{
    void         (* fn_ptr )(void **);//function
    void    **   args;               //function args
    fn_node *    next_fn;           //next node
    unsigned int id;               //function id( not needed )
};

struct periodic_node{          

    int             time;     //time til next group of functions
    periodic_node * next;    //Pointer to next node
    fn_node       * head_fn;//pointer to first function
};

typedef struct{

    periodic_node * head;
    periodic_node * tail;

}linked_fn;

//********************structs********************//

linked_fn make_linked_fn();
void add_fn(linked_fn * lkfn,void (* fn)(void **),void ** args,unsigned long int min,unsigned int id );
void start_fn(linked_fn lkfn);
void del_linked_fn(linked_fn lkfn);
periodic_node * create_periodic_node(periodic_node * next_prn);
void exec_node(periodic_node *node);

//START
//STOP TODO HOW DO I MAKE IT STOOOOOOOOOOOP

#endif

//Implementation

#ifdef _PeriodicFn_Imp_ //#define _PeriodicFn_Imp_
#endif
