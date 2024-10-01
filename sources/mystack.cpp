#include <stdlib.h>
#include <stdio.h>

#include "mystack.h"

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

void stackReduce(stack_t * stk);
void stackEnlarge(stack_t * stk);
void stackResize(stack_t * stk, size_t newcap);

stack_t stackCtor(size_t start_cap)
{
    stack_t stk ={};
    stk.data = (stack_elem_t *) calloc(start_cap, sizeof(stack_elem_t));
    stk.size = 0;
    stk.capacity = start_cap;
    return stk;
}

void stackDtor(stack_t * stk)
{
    free(stk->data);
}

void stackResize(stack_t * stk, size_t newcap)
{
    stk->data = (stack_elem_t *) realloc(stk->data, newcap * sizeof(stack_elem_t));
    stk->capacity = newcap;
}

void stackEnlarge(stack_t * stk)
{
    stackResize(stk, MAX(MINSTACKDIFF, 2 * stk->capacity));
}

void stackReduce(stack_t * stk)
{
    if (stk->capacity > MINSTACKDIFF)
        stackResize(stk, stk->capacity / 2);
}

stack_elem_t stackPop(stack_t * stk)
{
    stack_elem_t val = stk->data[--(stk->size)];
    if (stk->size <= stk->capacity / 4)
        stackReduce(stk);
    return val;
}

void stackPush(stack_t * stk, stack_elem_t val)
{
    if (stk->size == stk->capacity)
        stackEnlarge(stk);
    stk->data[stk->size] = val;
    stk->size++;
}
