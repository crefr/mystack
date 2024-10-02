#ifndef MYSTACK_INCLUDED
#define MYSTACK_INCLUDED

#include <stdint.h>

typedef enum {STACK_OK = 0, STACK_UNDEF_ERROR, STACK_HASH_ERROR} stackstatus;

typedef double stack_elem_t;

typedef struct {
    stack_elem_t * data;
    size_t size;
    size_t capacity;
    uint32_t hash;
} stack_t;

#define MINSTACKDIFF 16

stack_t stackCtor(size_t start_cap);
void stackDtor(stack_t * stk);
stack_elem_t stackPop(stack_t * stk);
void stackPush(stack_t * stk, stack_elem_t val);
uint32_t stackHash(stack_t * stk);
stackstatus stackOK(stack_t * stk);

#endif
