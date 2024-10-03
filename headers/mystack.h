#ifndef MYSTACK_INCLUDED
#define MYSTACK_INCLUDED

#include <stdint.h>

#define STACK_DEBUG

#ifdef STACK_DEBUG
    #define IF_STACK_DEBUG(...) __VA_ARGS__
    typedef uint64_t canary_t;
    #define CANARY1 0xC9EF9228
    #define CANARY2 0x228C9EF9
#else
    #define IF_STACK_DEBUG(...)
#endif

typedef enum {
    STACK_OK = 0,
    STACK_UNDEF_ERROR,
    STACK_HASH_ERROR,
    STACK_DATA_ERROR,
    STACK_SIZE_TOOBIG,
    STACK_STCANARYERROR
} stackstatus;

typedef double stack_elem_t;
typedef uint32_t hash_t;

// do not move uint32_t hash, it must be first (to skip it while calculating hash)
typedef struct {
    hash_t hash;
    canary_t structcanary1;
    stack_elem_t * data;
    size_t size;
    size_t capacity;
    canary_t structcanary2;
} stack_t;

#define MINSTACKDIFF 16

IF_STACK_DEBUG(
int checkIfStructCanariesOK(stack_t * stk);

)

stack_t stackCtor(size_t start_cap);
void stackDtor(stack_t * stk);

stack_elem_t stackPop(stack_t * stk);
void stackPush(stack_t * stk, stack_elem_t val);

uint32_t stackGetHash(stack_t * stk);

stackstatus stackOK(stack_t * stk);
void stackDump(stack_t * stk);

#endif
