#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "mystack.h"
#include "hash.h"
#include "logger.h"

#define STACKASSERT(stkptr, expr)                                                                             \
    do{                                                                                                       \
        if (!expr){                                                                                           \
            LOGPRINTWITHTIME(0, "---\\/\n<<<<<<<<STACK ERROR>>>>>>>>");                                       \
            PRINTFANDLOG(0, "Assertion failed:\n\t{" #expr "}\n\tFILE %s, in FUNCTION \"%s\", LINE %d",       \
                         __FILE__, __PRETTY_FUNCTION__, __LINE__);                                            \
            stackDump(stkptr);                                                                                \
            logExit();                                                                                        \
            exit(1);                                                                                          \
        }                                                                                                     \
    }while(0)

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

void stackReduce(stack_t * stk);
void stackEnlarge(stack_t * stk);
void stackResize(stack_t * stk, size_t newcap);

stack_t stackCtor(size_t start_cap)
{
    stack_t stk = {};
    stk.structcanary1 = CANARY1;
    stk.data = (stack_elem_t *) calloc(start_cap, sizeof(stack_elem_t));
    stk.size = 0;
    stk.capacity = start_cap;
    stk.structcanary2 = CANARY2;

    stk.hash = stackGetHash(&stk);
    return stk;
}

void stackDtor(stack_t * stk)
{
    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->hash = 0;
}

void stackResize(stack_t * stk, size_t newcap)
{
    stk->data = (stack_elem_t *) realloc(stk->data, newcap * sizeof(stack_elem_t));
    stk->capacity = newcap;
    stk->hash = stackGetHash(stk);
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
    STACKASSERT(stk, stk->size != 0);
    stack_elem_t val = stk->data[--(stk->size)];
    if (stk->size <= stk->capacity / 4)
        stackReduce(stk);
    stk->hash = stackGetHash(stk);
    return val;
}

void stackPush(stack_t * stk, stack_elem_t val)
{
    if (stk->size == stk->capacity)
        stackEnlarge(stk);
    stk->data[stk->size] = val;
    stk->size++;
    stk->hash = stackGetHash(stk);
}

stackstatus stackOK(stack_t * stk)
{
    IF_STACK_DEBUG(
    if (!checkIfStructCanariesOK(stk))
        return STACK_STCANARYERROR;
    )

    if (stk->data == NULL)
        return STACK_DATA_ERROR;
    if (stk->size >= stk->capacity)
        return STACK_SIZE_TOOBIG;
    if (stk->hash != stackGetHash(stk))
        return STACK_HASH_ERROR;
    return STACK_OK;
}

hash_t stackGetHash(stack_t * stk)
{
    uint32_t datahash   = MurMur32Hash(stk->data, stk->capacity * sizeof(stack_elem_t), 0);
    char * structstartptr = (char *)stk + sizeof(hash_t);
    uint32_t structhash = MurMur32Hash(structstartptr, sizeof(stack_t) - sizeof(hash_t), 0);
    uint32_t hash = (datahash >> 1) + (structhash >> 1);
    return hash;
}

void stackDump(stack_t * stk)
{
    //logPrintTime(LOG_DEBUG);
    logPrint(LOG_DEBUG, "-----------STACK DUMP-----------");
    logPrint(LOG_DEBUG, "stack_t[%p]{", stk);
    logPrint(LOG_DEBUG, "\tsize = %lu", stk->size);
    logPrint(LOG_DEBUG, "\tcapacity = %lu", stk->capacity);
    logPrint(LOG_DEBUG, "\thash = %08X", stk->hash);
    logPrint(LOG_DEBUG, "\tdata[%p]", stk->data);
    if (stk->data != NULL){
        logPrint(LOG_DEBUG, "\t{");
        for (size_t index = 0; index < stk->capacity; index++){
            logPrint(LOG_DEBUG, "\t\t[%lu] = %lg", index, stk->data[index]);
        }
        logPrint(LOG_DEBUG, "\t}");
    }
    logPrint(LOG_DEBUG, "}\n---------STACK DUMP END---------");
}

#ifdef STACK_DEBUG
int checkIfStructCanariesOK(stack_t * stk)
{
    if (stk->structcanary1 != CANARY1 ||
        stk->structcanary2 != CANARY2)
        return 0;
    return 1;
}
#endif
