#include <stdio.h>
#include <stdlib.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG_PLUS);
    stack_t mystk = stackCtor(0);

    for (int32_t index = 0; index < 128; index++){
        stackPush(&mystk, index);
    }

    for (size_t index = 0; index < 96; index++){
        printf("%d\n", stackPop(&mystk));
    }
    stackDump(&mystk);
    stackDtor(&mystk);
    logExit();
    return 0;
}
