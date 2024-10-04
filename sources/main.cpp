#include <stdio.h>
#include <stdlib.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG_PLUS);
    stack_t mystk = stackCtor(0);

    for (size_t index = 0; index < 15; index++){
        stackPush(&mystk, (double) ((index * 157) % 21));
    }
    for (size_t index = 0; index < 10; index++){
        printf("%lg\n", stackPop(&mystk));
    }
    for (size_t index = 0; index < 15; index++){
        stackPush(&mystk, (double) ((index * 156) % 21));
    }
    for (size_t index = 0; index < 10; index++){
        printf("%lg\n", stackPop(&mystk));
    }
    stackDump(&mystk);
    stackDtor(&mystk);
    logExit();
    return 0;
}
