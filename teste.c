#include <stdio.h>
#include "PeriodicFn.h"

void fn1(char *str){

    printf("&&str1fn1:%s&&\n",str);
}

void fn2(char *str1,char *str2){

    printf("**str1:%s\nstr2:%s**\n",str1,str2);
}

void fn3(char *str1,char *str2){

    printf("//fn3str1:%s\nstr2:%s//\n`",str1,str2);
}

void fn1gen(void **args){

    fn1(args[0]);
}
void fn2gen(void **args){

    fn2(args[0],args[1]);
}
void fn3gen(void **args){

    fn3(args[0],args[1]);
}

int main(void){

    const char *str1 = "str1",*str2 = "str2",*str3 = "str3";
    void * agrs1[1] = {(void*)str1};
    void * agrs2[2] = {(void*)str1,(void*)str2};
    void * agrs3[2] = {(void*)str1,(void*)str3};
    linked_fn lkfn = make_linked_fn();
    periodic_node * prn_aux;

    
    add_fn( &lkfn, *fn2gen, agrs2 , 90 );
    add_fn( &lkfn, *fn1gen, agrs1 , 35 );
    //exec_node(prn_aux);
    //prn_aux = prn_aux->next;
    add_fn( &lkfn, *fn2gen, agrs2 , 90 );
    //exec_node(prn_aux);
    add_fn( &lkfn, *fn3gen, agrs3 , 35 );
    //exec_node(lkfn.head);
    prn_aux = lkfn.head;
   // printf("NODE0 TIME:%d\n",prn_aux->time);
   // printf("ADDR: %p\n",lkfn.head);
    for(int i = 0;i < 2;++i){

        printf("ADDR: %p\n",prn_aux);
        printf("NODE%d TIME:%d\n",i,prn_aux->time);
        exec_node(prn_aux);
        prn_aux = prn_aux->next;
    }
    return 0;
}
