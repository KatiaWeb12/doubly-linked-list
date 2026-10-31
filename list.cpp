#include "list.h"
#include "graphics.cpp"
#include "formLogs.cpp"

int main(){
    #ifdef LIST_DEBUG
    createLogSpace();
    #endif

    list_t list = {};

    listLoad(&list, LIST_CAPACITY);

    listAddElem(&list, 0, 10);
    listAddElem(&list, 1, 20);
    listAddElem(&list, 2, 30);
    listAddElem(&list, 3, 40);
    listAddElem(&list, 4, 50);
    listAddElem(&list, 5, 60);
    listAddElem(&list, 6, 70);
    listAddElem(&list, 7, 80);
    listAddElem(&list, 8, 90);

    listAddElem(&list, 3, 35);
    listDelElem(&list, 7);

    #ifdef LIST_DEBUG
    launchLogFile("log.html");
    #endif
}

ErrorCode listLoad(list_t* list, const size_t listCapacity){

    list->capacity = listCapacity;
    list->head = 0;
    list->tail = 0;
    list->free = 1;

    listArrElem* memoryArr = (listArrElem*)calloc(listCapacity, sizeof(listArrElem));
    if (memoryArr == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "", ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);
        return ERR_OUT_OF_MEMORY;
    }
    list->listArr = memoryArr;

    list->listArr[0].value = LIST_POISON;
    list->listArr[0].next = 0;
    list->listArr[0].prev= 0;

    for(size_t i = 1; i < LIST_CAPACITY; i++){
        list->listArr[0].value = LIST_POISON;
        list->listArr[i].next = (i + 1 < listCapacity) ? (int)(i + 1) : 0;
        list->listArr[i].prev = -1;
    }

    #ifdef LIST_DEBUG
        list->debugInfo = {
            "ListData",
            __FILE__,
            __func__,
            __LINE__
        };
        htmlListDump("log.html", list);
    #endif

    return ERR_OK;

}

ErrorCode listAddElem(list_t* list, const size_t indexAfter, const list_elem_t value){

    assert(list);

    if (indexAfter > list->capacity){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "indexAfter incorrect", ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);
        return ERR_INVALID_ARGUMENT;
    }

    ErrorCode error = ERR_OK;
    size_t emptyIndex = getFreeEl(list, &error);
    if (error != ERR_OK) return error;

    size_t nextIndex = list->listArr[indexAfter].next;

    list->listArr[emptyIndex].value = value;
    list->listArr[emptyIndex].next = nextIndex;
    list->listArr[emptyIndex].prev = indexAfter;

    list->listArr[indexAfter].next = emptyIndex;
    list->listArr[nextIndex].prev = emptyIndex;

    list->head = list->listArr[0].next;
    list->tail = list->listArr[0].prev;

    #ifdef LIST_DEBUG
    htmlListDump("log.html", list);
    #endif

    return ERR_OK;
}

size_t getFreeEl(list_t* list, ErrorCode* error){

    assert(list);
    assert(error);

    size_t i = list->free;

    list->free = list->listArr[i].next;

    *error = ERR_OK;

    return i;
}

ErrorCode listDelElem(list_t* list, const size_t indexDel){

    assert(list);

    if (indexDel > list->capacity){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "indexDel incorrect", ERR_OUT_OF_MEMORY);
        printErrorIntoConsole(&debugLogInfo);
        return ERR_INVALID_ARGUMENT;
    }

    size_t prevIndex = list->listArr[indexDel].prev;
    size_t nextIndex = list->listArr[indexDel].next;

    list->listArr[prevIndex].next = list->listArr[indexDel].next;
    list->listArr[nextIndex].prev = list->listArr[indexDel].prev;

    list->listArr[indexDel].value = LIST_POISON;
    list->listArr[indexDel].next = list->free;
    list->listArr[indexDel].prev = -1;

    list->free = indexDel;

    list->head = list->listArr[0].next;
    list->tail = list->listArr[0].prev;

    #ifdef LIST_DEBUG
    htmlListDump("log.html", list);
    #endif

    return ERR_OK;
}



