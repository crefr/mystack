#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "mystack.h"
#include "hash.h"
#include "logger.h"

#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

void stackReduce(stack_t * stk);
void stackEnlarge(stack_t * stk);
void stackResize(stack_t * stk, size_t newcap);

stack_t stackCtor(size_t start_cap)
{
    stack_t stk = {};
    IF_STACK_STRUCT_CANARIES_ON(stk.structcanary1 = CANARY1;)
    stk.data = (stack_elem_t *) calloc(start_cap, sizeof(stack_elem_t));
    stk.size = 0;
    stk.capacity = start_cap;
    stk.errno = STACK_OK;
    stackPoisonRest(&stk);
    IF_STACK_STRUCT_CANARIES_ON(stk.structcanary2 = CANARY2;)
    IF_STACK_HASH_ON(stackUpdateHash(&stk);)
    STACKASSERT(&stk, stackOK(&stk) == STACK_OK);
    return stk;
}

void stackDtor(stack_t * stk)
{
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    IF_STACK_HASH_ON(stk->hash = 0);
}

void stackResize(stack_t * stk, size_t newcap)
{
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
    stk->data = (stack_elem_t *) realloc(stk->data, newcap * sizeof(stack_elem_t));
    stk->capacity = newcap;
}

void stackEnlarge(stack_t * stk)
{
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
    stackResize(stk, MAX(MINSTACKDIFF, 2 * stk->capacity));
    stackPoisonRest(stk);
    IF_STACK_HASH_ON(stackUpdateHash(stk));
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
}

void stackReduce(stack_t * stk)
{
    if (stk->capacity > MINSTACKDIFF)
        stackResize(stk, stk->capacity / 2);
    IF_STACK_HASH_ON(stackUpdateHash(stk));
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
}

stack_elem_t stackPop(stack_t * stk)
{
    STACKASSERT(stk, stk->size != 0);
    stack_elem_t val = stk->data[--(stk->size)];
    if (stk->size <= stk->capacity / 4)
        stackReduce(stk);
    stk->data[stk->size] = stackpoison;
    IF_STACK_HASH_ON(stackUpdateHash(stk));
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
    return val;
}

void stackPush(stack_t * stk, stack_elem_t val)
{
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
    if (stk->size == stk->capacity)
        stackEnlarge(stk);
    stk->data[stk->size] = val;
    stk->size++;
    IF_STACK_HASH_ON(stackUpdateHash(stk));
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
}

void stackPoisonRest(stack_t * stk)
{
    for (size_t index = stk->size; index < stk->capacity; index++)
        stk->data[index] = (stack_elem_t) stackpoison;
}

void stackDump(stack_t * stk)
{
    //logPrintTime(LOG_DEBUG);
    logPrint(LOG_DEBUG, "-----------STACK DUMP-----------");
    logPrint(LOG_DEBUG, "stack_t[%p]{", stk);
    logPrint(LOG_DEBUG, "\terrno = %d", stk->errno);
    logPrint(LOG_DEBUG, "\tsize = %lu", stk->size);
    logPrint(LOG_DEBUG, "\tcapacity = %lu", stk->capacity);
IF_STACK_HASH_ON(
    logPrint(LOG_DEBUG, "\thash = %08X", stk->hash);
)
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

stackstatus stackOK(stack_t * stk)
{
IF_STACK_STRUCT_CANARIES_ON(
    if (!checkIfStructCanariesOK(stk))
        stk->errno = STACK_STCANARYERROR;
)
IF_STACK_HASH_ON(
    if (stk->hash != stackGetHash(stk))
        stk->errno = STACK_HASH_ERROR;
)

    if (stk->data == NULL)
        stk->errno = STACK_DATA_ERROR;
    if (stk->size >= stk->capacity && (stk->capacity != 0))
        stk->errno = STACK_SIZE_TOOBIG;
    return stk->errno;
}


IF_STACK_HASH_ON(
hash_t stackGetHash(stack_t * stk)
{
    uint32_t datahash   = MurMur32Hash(stk->data, stk->capacity * sizeof(stack_elem_t), 0);
    char * structstartptr = (char *)stk + sizeof(hash_t);
    uint32_t structhash = MurMur32Hash(structstartptr, sizeof(stack_t) - sizeof(hash_t), 0);
    uint32_t hash = (datahash >> 1) + (structhash >> 1);
    return hash;
}
)

IF_STACK_HASH_ON(
void stackUpdateHash(stack_t * stk)
{
    stk->hash = stackGetHash(stk);
    STACKASSERT(stk, stackOK(stk) == STACK_OK);
}
)

IF_STACK_STRUCT_CANARIES_ON(
int checkIfStructCanariesOK(stack_t * stk)
{
    if (stk->structcanary1 != CANARY1 ||
        stk->structcanary2 != CANARY2)
        return 0;
    return 1;
}
)
