#include <stdio.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG_PLUS);

    stack_t mystk = stackCtor(0);

    for (size_t index = 0; index < 2048; index++){
        stackPush(&mystk, (double) ((index * 156) % 21));
        LOGPRINT(LOG_DEBUG_PLUS, "pushing >> cap: %lu val: %lg" , mystk.capacity, (double) ((index * 156) % 21));
        LOGPRINT(LOG_DEBUG_PLUS, "datahash: 0x%08X", stackHash(&mystk));
        LOGPRINT(LOG_DEBUG_PLUS, (stackOK(&mystk) == STACK_OK) ? "stackOK: OK!" : "stackOK: ERROR!");
    }

    printf("hash1: %08X\n", stackHash(&mystk));
    printf("hash2: %08X\n", stackHash(&mystk));
    for (size_t index = 0; index < 2048; index++){
        LOGPRINT(LOG_DEBUG_PLUS, "poping  << cap: %lu val: %lg", mystk.capacity, stackPop(&mystk));
    }

    stackDtor(&mystk);
    LOGEXIT();
    return 0;
}
