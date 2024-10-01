#ifndef MYSTACK_INCLUDED
#define MYSTACK_INCLUDED

typedef double stack_elem_t;

typedef struct {
    stack_elem_t * data;
    size_t size;
    size_t capacity;
} stack_t;

#define MINSTACKDIFF 16

stack_t stackCtor(size_t start_cap);
void stackDtor(stack_t * stk);
stack_elem_t stackPop(stack_t * stk);
void stackPush(stack_t * stk, stack_elem_t val);

#endif
