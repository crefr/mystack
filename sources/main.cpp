#include <stdio.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG);

    stack_t mystk = stackCtor(0);

    for (size_t index = 0; index < 2048; index++){
        stackPush(&mystk, (double) ((index * 156) % 21));
    }
    stackDump(&mystk);
    for (size_t index = 0; index < 10; index++){
        LOGPRINT(LOG_DEBUG_PLUS, "poping  << cap: %zu val: %lg", mystk.capacity, stackPop(&mystk));
    }
    stackDump(&mystk);

    stackDtor(&mystk);
    LOGEXIT();
    return 0;
}
