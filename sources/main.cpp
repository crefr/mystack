#include <stdio.h>
#include <stdlib.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG);
    stack_t mystk = stackCtor(0);

    for (size_t index = 0; index < 150; index++){
        stackPush(&mystk, (double) ((index * 156) % 21));
    }
    stackDump(&mystk);
    for (size_t index = 0; index < 149; index++){
        printf("%lg\n", stackPop(&mystk));
    }
    stackDump(&mystk);
    stackDtor(&mystk);
    logExit();
    return 0;
}
