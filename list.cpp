#include <TXLib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>

#include "../universal-features/error.h"
#include "../universal-features/colors.h"
#include "../universal-features/useful-functions.h"
#include "list.h"

int main(){

    list_t list = {};
    listLoad(&list, LIST_CAPACITY);

    #ifdef LIST_DEBUG
        list.debugInfo = {
            "ListData",
            __FILE__,
            __func__,
            __LINE__
        };
    #endif

    listDump(&list);
}

ErrorCode listLoad(struct list_t* list, const size_t listCapacity){

    list->capacity = listCapacity;
    list->head = 1;
    list->tail = 1;
    list->free = 0;

    listArrElem* memoryArr = (listArrElem*)calloc(listCapacity, sizeof(listArrElem));
    if (memoryArr == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "", ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);
        return ERR_OUT_OF_MEMORY;
    }
    list->listArr = memoryArr;


    list->listArr[0].value = 0;
    list->listArr[0].next = 0;
    list->listArr[0].prev= 0;

    for(size_t i = 1; i < LIST_CAPACITY; i++){

        list->listArr[i].next = -1;
        list->listArr[i].prev = -1;
    }

    return ERR_OK;

}

#ifdef LIST_DEBUG
ErrorCode listDump(const list_t* list)
{

    assert(list);

    printf("\nlist_t '%s'[%p] created by %s() at %s:%u\n",
            list->debugInfo.name,
            list,
            list->debugInfo.function,
            list->debugInfo.file,
            list->debugInfo.line);

    printf("{\n");

    printf("\tcapacity = %u\n", list->capacity);

    printf("\thead = %u\n", list->head);
    printf("\ttail = %u\n", list->tail);
    printf("\tfree = %u\n", list->free);

    printIndexes(list->listArr);

    printf("\tlistArr[%p] {\n", list->listArr);
    printArr(list->listArr);
    printf("\t}\n");

    printf("}\n");

    getchar();

    return ERR_OK;
}

void printIndexes(listArrElem* arr){

    assert(arr);

    putchar('\t');

    for(size_t i = 0; i < LIST_CAPACITY; i++){
        printf("%2u ", i);
    }

    putchar('\n');

}

void printArr(listArrElem* arr){

    assert(arr);

    for(size_t i = 0; i < LIST_CAPACITY; i++){
        printArrEl(i);
        printArrEl(arr[i].value);
        printArrEl(arr[i].next);
        printArrEl(arr[i].prev);
        putchar('\n');
    }

}

void printArrEl(list_elem_t value){

    putchar('\t');

    if(value == LIST_POISON){
        printf(COLOR_MAGENTA "%2d " COLOR_RESET, LIST_POISON);
    }
    else{
        printf("%2d ", value);
    }

    return;
}
#endif
