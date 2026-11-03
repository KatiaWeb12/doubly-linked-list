#include "list.h"
#include "graphics.cpp"
#include "formLogs.cpp"
#include "verify.cpp"

int main(int argc, char* argv[]){

    return doneTest(argc, argv);
    //return brokenTest(argc, argv);
    //return doneTestInsertBeforeHead(argc, argv);

}

ErrorCode doneTest(int argc, char* argv[]){

    list_t list = {};

    #ifdef LIST_DEBUG
        createLogSpace(&list);

        if (getVerifyLevel(argc, argv, &list) != ERR_OK) return ERR_OK;
    #endif

    listInit(&list, LIST_CAPACITY);

    size_t addedIndex = 0;

    addedIndex = listAddElem(&list, 0, 10);
    addedIndex = listAddElem(&list, addedIndex, 20);
    addedIndex = listAddElem(&list, addedIndex, 30);
    addedIndex = listAddElem(&list, addedIndex, 40);
    addedIndex = listAddElem(&list, addedIndex, 50);
    addedIndex = listAddElem(&list, addedIndex, 60);
    addedIndex = listAddElem(&list, addedIndex, 70);
    addedIndex = listAddElem(&list, addedIndex, 80);
    addedIndex = listAddElem(&list, addedIndex, 90);

    addedIndex = listAddElem(&list, 3, 35);
    listDelElem(&list, 7);

    #ifdef LIST_DEBUG
        listDestroy(&list);
        launchLogFile();
    #endif

    return ERR_OK;
}

ErrorCode doneTestInsertBeforeHead(int argc, char* argv[]){

    list_t list = {};

    #ifdef LIST_DEBUG
        createLogSpace(&list);

        if (getVerifyLevel(argc, argv, &list) != ERR_OK) return ERR_OK;
    #endif

    listInit(&list, LIST_CAPACITY);

    size_t addedIndex = 0;

    addedIndex = listAddElem(&list, 0, 10);
    addedIndex = listAddElem(&list, addedIndex, 20);
    addedIndex = listAddElem(&list, addedIndex, 30);
    addedIndex = listAddElem(&list, addedIndex, 40);
    addedIndex = listAddElem(&list, addedIndex, 50);
    addedIndex = listAddElem(&list, addedIndex, 60);
    addedIndex = listAddElem(&list, addedIndex, 70);

    addedIndex = listAddElem(&list, addedIndex, 80);
    addedIndex = listAddElem(&list, addedIndex, 90);

    addedIndex = listAddElem(&list, 3, 35);
    listDelElem(&list, 7);

    addedIndex = listAddElem(&list, 0, 5);

    #ifdef LIST_DEBUG
        listDestroy(&list);
        launchLogFile();
    #endif

    return ERR_OK;
}

ErrorCode brokenTest(int argc, char* argv[]){

    list_t list = {};

    #ifdef LIST_DEBUG
        createLogSpace(&list);

        if (getVerifyLevel(argc, argv, &list) != ERR_OK) return ERR_OK;
    #endif

    listInit(&list, LIST_CAPACITY);

    size_t addedIndex = 0;

    addedIndex = listAddElem(&list, 0, 10);
    addedIndex = listAddElem(&list, addedIndex, 20);
    addedIndex = listAddElem(&list, addedIndex, 30);
    addedIndex = listAddElem(&list, addedIndex, 40);
    addedIndex = listAddElem(&list, addedIndex, 50);
    addedIndex = listAddElem(&list, addedIndex, 60);
    addedIndex = listAddElem(&list, addedIndex, 70);

    list.listArr[3].next = 56;

    addedIndex = listAddElem(&list, addedIndex, 80);
    addedIndex = listAddElem(&list, addedIndex, 90);

    addedIndex = listAddElem(&list, 3, 35);
    listDelElem(&list, 7);


    #ifdef LIST_DEBUG
        listDestroy(&list);
        launchLogFile();
    #endif

    return ERR_OK;
}




ErrorCode listInit(list_t* list, const size_t listCapacity){

    list->capacity = listCapacity;
    list->head = 0;
    list->tail = 0;
    list->free = 1;

    listArrElem* memoryArr = (listArrElem*)calloc(listCapacity, sizeof(listArrElem));
    if (memoryArr == NULL){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "", ERR_OUT_OF_MEMORY);
        printErrorIntoLog(list->logFilePtr, &debugLogInfo);
        return ERR_OUT_OF_MEMORY;
    }
    list->listArr = memoryArr;

    list->listArr[0].value = LIST_POISON;
    list->listArr[0].next = 0;
    list->listArr[0].prev= 0;

    for(size_t i = 1; i < LIST_CAPACITY; i++){
        list->listArr[i].value = LIST_POISON;
        list->listArr[i].next = (i + 1 < listCapacity) ? (int)(i + 1) : 0;
        list->listArr[i].prev = -1;
    }

    #ifdef LIST_DEBUG
        list->debugInfo = {};
        list->debugInfo.name = "ListData";
        list->debugInfo.file = __FILE__;
        list->debugInfo.function = __func__;
        list->debugInfo.line = __LINE__;
        list->debugInfo.logPictureNum = 0;

        loadDump(list);
        VERIFY_OK(list, __func__);
    #endif

    return ERR_OK;

}

size_t listAddElem(list_t* list, const size_t indexAfter, const list_elem_t value){

    #ifdef LIST_DEBUG
        addDump(list, indexAfter, value, BEFORE);
        VERIFY_OK(list, __func__);
    #endif

    assert(list);

    if (indexAfter >= list->capacity){
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "indexAfter incorrect", ERR_OUT_OF_MEMORY);
        printErrorIntoLog(list->logFilePtr, &debugLogInfo);
        abort();
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
        addDump(list, indexAfter, value, AFTER);
        VERIFY_OK(list, __func__);
    #endif

    return emptyIndex;
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
        printErrorIntoLog(list->logFilePtr, &debugLogInfo);
        return ERR_INVALID_ARGUMENT;
    }

    size_t prevIndex = list->listArr[indexDel].prev;
    size_t nextIndex = list->listArr[indexDel].next;

    list_elem_t deletingValue = list->listArr[indexDel].value;

    #ifdef LIST_DEBUG
        deleteDump(list, deletingValue, BEFORE);
        VERIFY_OK(list, __func__);
    #endif

    list->listArr[prevIndex].next = list->listArr[indexDel].next;
    list->listArr[nextIndex].prev = list->listArr[indexDel].prev;

    list->listArr[indexDel].value = LIST_POISON;
    list->listArr[indexDel].next = list->free;
    list->listArr[indexDel].prev = -1;

    list->free = indexDel;

    list->head = list->listArr[0].next;
    list->tail = list->listArr[0].prev;

    #ifdef LIST_DEBUG
        deleteDump(list, deletingValue, AFTER);
        VERIFY_OK(list, __func__);
    #endif

    return ERR_OK;
}

ErrorCode listDestroy(list_t* list) {

    assert(list);

    #ifdef LIST_DEBUG
        destroyDump(list);
        VERIFY_OK(list, __func__);

        if (list->logFilePtr != NULL) {

            FCLOSE(list->logFilePtr);
            list->logFilePtr = NULL;
        }
    #endif

    free(list->listArr);
    list->listArr = NULL;

    list->capacity = 0;
    list->head = 0;
    list->tail = 0;
    list->free = 0;

    #ifdef LIST_DEBUG
        list->debugInfo = {};
        list->verifyLevel = VERIFY_OFF;
    #endif

    return ERR_OK;
}




