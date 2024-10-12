#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mystack.h"
#include "logger.h"
#include "hash.h"

int main()
{
    logStart("log.txt", LOG_DEBUG_PLUS);
    stack_t mystk = stackCtor(sizeof(mystk) / sizeof(int32_t) - 1);
    int32_t* orig_data = (int32_t*)mystk.data;
    for (int32_t index = 1; index < sizeof(mystk) / sizeof(index); index++){
        if (index == (int32_t)((int32_t*)&mystk.size - (int32_t*)&mystk))
        {
            stackPush(&mystk, 7);
            stackPush(&mystk, 0);
            index++;
            continue;
        }

        stackPush(&mystk, *((int32_t*)&mystk + index));
    }
    stackDump(&mystk);

    printf("[LINE %d]: struct_hash = %X, data_hash = %X ",
        __LINE__,
        MurMur32Hash((char *)(&mystk) + sizeof(hash_t), sizeof(stack_t ) - sizeof(hash_t), 0),
        MurMur32Hash(mystk.data, mystk.capacity * sizeof(stack_elem_t), 0));

    printf("data = %p, hash = %X, c1 = %lX, c2 = %lX, size = %lu, cap = %lu, errno = %d\n",
        mystk.data, mystk.hash, mystk.structcanary1, mystk.structcanary2, mystk.size, mystk.capacity, mystk.errNo);

    for (size_t index = 1; index < sizeof(mystk) / sizeof(index); index++){
        int32_t temp = 0;
        memcpy(&temp, (int32_t*)&mystk + index, sizeof(int32_t));
        memcpy((int32_t*)&mystk + index, orig_data + index - 1, sizeof(int32_t));
        memcpy(orig_data + index - 1, &temp, sizeof(int32_t));
    }
    printf("new hash: %X\n", stackGetHash(&mystk));

    stackPop(&mystk);
    printf("[LINE %d]: struct_hash = %X, data_hash = %X ",
        __LINE__,
        MurMur32Hash((char *)(&mystk) + sizeof(hash_t), sizeof(stack_t ) - sizeof(hash_t), 0),
        MurMur32Hash(mystk.data, mystk.capacity * sizeof(stack_elem_t), 0));

    printf("data = %p, hash = %X, c1 = %lX, c2 = %lX, size = %lu, cap = %lu, errno = %d\n",
        mystk.data, mystk.hash, mystk.structcanary1, mystk.structcanary2, mystk.size, mystk.capacity, mystk.errNo);

    mystk.data += 5;
    stackDump(&mystk);
    stackDtor(&mystk);

    logExit();
    printf("end\n");
    return 0;
}
