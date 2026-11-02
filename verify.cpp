#ifdef LIST_DEBUG

struct error_debug_verify {
    const char* time;
    const char* file;
    const char* function;
    size_t line;
    ListVerifyCode errCode;
};

ListVerifyCode listVerify(const char* logFileName, const list_t* list){

    FILE* file = fopen(logFileName, "a");
    assert(file == NULL);

    error_debug_verify debugLogInfo = {};
    ERROR_STRUCT_VERIFY_FORMAT(&debugLogInfo, __func__, __LINE__, LIST_OK);

    if(list == NULL){

        debugLogInfo.errCode = LIST_NULL;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_NULL;
    }
    if(list->listArr == NULL){

        debugLogInfo.errCode = LIST_NULL_ARRAY;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_NULL_ARRAY;
    }
    if(list->capacity <= 1){

        debugLogInfo.errCode = LIST_INVALID_CAPACITY;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_CAPACITY;
    }
    if(list->head > list->capacity){

        debugLogInfo.errCode = LIST_INVALID_HEAD;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_HEAD;
    }
    if(list->tail > list->capacity){

        debugLogInfo.errCode = LIST_INVALID_TAIL;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_TAIL;
    }
    if(list->tail > list->capacity){

        debugLogInfo.errCode = LIST_INVALID_FREE;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_FREE;
    }
    if(list->listArr[0].value != LIST_POISON){

        debugLogInfo.errCode = LIST_INVALID_EXTRA_EL;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_EXTRA_EL;
    }
    if(list->listArr[0].next != (list_elem_t)list->head){

        debugLogInfo.errCode = LIST_INVALID_EXTRA_EL;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_EXTRA_EL;
    }
    if(list->listArr[0].prev != (list_elem_t)list->tail){

        debugLogInfo.errCode = LIST_INVALID_EXTRA_EL;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_INVALID_EXTRA_EL;
    }

    size_t activeCount = 0;
    size_t currentElem = list->head;
    size_t myPrev = 0;

    while(currentElem != 0){

        activeCount++;

        if(activeCount >= list->capacity){

            debugLogInfo.errCode = LIST_CYCLE;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_CYCLE;
        }
        if(currentElem >= list->capacity){

            debugLogInfo.errCode = LIST_INVALID_NEXT;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_NEXT;
        }

        listArrElem activeElem = list->listArr[currentElem];
        if(activeElem.value == LIST_POISON){

            debugLogInfo.errCode = LIST_ACTIVE_POISON;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_ACTIVE_POISON;
        }
        if((activeElem.prev < 0 || (size_t)activeElem.prev >= list->capacity)){

            debugLogInfo.errCode = LIST_INVALID_PREV;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_PREV;
        }
        if(activeElem.next < 0 || (size_t)activeElem.next >= list->capacity){

            debugLogInfo.errCode = LIST_INVALID_NEXT;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_NEXT;
        }
        if(list->listArr[myPrev].next == (int)currentElem){

            debugLogInfo.errCode = LIST_BAD_PREV_NEXT;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_BAD_PREV_NEXT;
        }
        if(list->listArr[activeElem.next].prev == (int)currentElem){

            debugLogInfo.errCode = LIST_BAD_NEXT_PREV;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_BAD_NEXT_PREV;
        }

        myPrev = currentElem;
        currentElem = (list_elem_t)activeElem.next;
    }
    if(myPrev != list->tail){

            debugLogInfo.errCode = LIST_INVALID_TAIL;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_TAIL;
    }

    currentElem = list->free;
    size_t freeCount = 0;

    while(currentElem != 0){

        freeCount++;

        if(freeCount >= list->capacity){

            debugLogInfo.errCode = LIST_CYCLE;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_FREE_CYCLE;
        }
        if(currentElem >= list->capacity){

            debugLogInfo.errCode = LIST_INVALID_NEXT;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_FREE;
        }

        listArrElem elemFree = list->listArr[currentElem];

        if(elemFree.prev != -1){

            debugLogInfo.errCode = LIST_INVALID_FREE;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_FREE;
        }
        if(elemFree.value != LIST_POISON){

            debugLogInfo.errCode = LIST_INVALID_FREE;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_FREE;
        }
        if(elemFree.next < 0 || elemFree.next >= (int)list->capacity){

            debugLogInfo.errCode = LIST_INVALID_NEXT;
             printVerifyErrorIntoConsole(&debugLogInfo);
            return LIST_INVALID_NEXT;
        }

        currentElem = (size_t)elemFree.next;
    }

    if (activeCount + freeCount != list->capacity - 1){

        debugLogInfo.errCode = LIST_LOST_ELEMENT;
         printVerifyErrorIntoConsole(&debugLogInfo);
        return LIST_LOST_ELEMENT;
    }

    return LIST_OK;
}

void printVerifyErrorIntoConsole(struct error_debug_verify* debugLogInfo) {

    assert(debugLogInfo);

    printf("[%s] Function '%s' was completed with Error %d in file '%s' in line %d \n",
            debugLogInfo->time,
            debugLogInfo->function,
            debugLogInfo->errCode,
            debugLogInfo->file,
            debugLogInfo->line);

    return;
}

#endif
