#include <cstdlib>

static void printDebugInfoIntoLogFile(FILE* file, list_t* list);
static void printArr(FILE* file, listArrElem* arr);
static void printArrEl(FILE* file, const list_elem_t value);
static void printLinArr(FILE* file, const list_t* list);
static void addPictureIntoLog(FILE* file, list_t* list);

#ifdef LIST_DEBUG

ErrorCode createLogSpace(list_t* list){

    char timeBuffer[DIRECTORY_NAME_MAX_LENGTH] = "";
    getOperationTime(timeBuffer, DIRECTORY_NAME_MAX_LENGTH);

    char directoryName[DIRECTORY_NAME_MAX_LENGTH] = "log_";
    strcat(directoryName, timeBuffer);

    char directoryCreationCommand[DIRECTORY_NAME_MAX_LENGTH] = "mkdir ";
    strcat(directoryCreationCommand, directoryName);

    system(directoryCreationCommand);
    _chdir(directoryName);

    FILE* file = NULL;
    ErrorCode error = openLogFile(&file);
    if(error != ERR_OK) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "", error);
        printErrorIntoConsole(&debugLogInfo);
        return error;
    };

    list->logFilePtr = file;

    return ERR_OK;
}

ErrorCode htmlListDump(list_t* list){

    assert(list);

    FILE* file = list->logFilePtr;

    fprintf(file, "<link rel=\"stylesheet\" href=\"../styles.css\">");
    fprintf(file, "<pre>\n");

    printDebugInfoIntoLogFile(file, list);

    return ERR_OK;
}

static void printDebugInfoIntoLogFile(FILE* file, list_t* list){

    assert(list);
    assert(file);

    fprintf(file, "<div class=\"table-text\">\n"
                    "<table border=\"1\">"
                    "<caption>listArr [%p]: </caption>",
                    list->listArr);

    printArr(file, list->listArr);

    fprintf(file, "</table>\n"
                  "<div class=\"info\">\n");

    fprintf(file, "<p>\nlist_t '%s'[%p] created by %s() at %s:%u</p>",
            list->debugInfo.name,
            list,
            list->debugInfo.function,
            list->debugInfo.file,
            list->debugInfo.line);

    fprintf(file, "<p>capacity = %u</p>", list->capacity);
    // fprintf(file, "<p>head = %u</p>", list->head);
    // fprintf(file, "<p>tail = %u</p>", list->tail);
    // fprintf(file, "<p>free = %u</p>", list->free);

    fprintf(file, "</div>\n</div>\n");

    addPictureIntoLog(file, list);

    printLinArr(file, list);

    return;
}

static void printArr(FILE* file, listArrElem* arr){

    assert(arr);
    assert(file);

    fprintf(file, "\t\t<tr> \n"
                        "<th>Index</th> \n"
                        "<th>Value</th> \n"
                        "<th>Next</th> \n"
                        "<th>Prev</th> \n"
                        "</tr>\n");

    for(size_t i = 0; i < LIST_CAPACITY; i++){

        fprintf(file, "<tr>");

        printArrEl(file, i);
        printArrEl(file, arr[i].value);
        printArrEl(file, arr[i].next);
        printArrEl(file, arr[i].prev);

        fprintf(file, "</tr>");
        fprintf(file, "\n");
    }

}

static void printArrEl(FILE* file, const list_elem_t value){

    assert(file);
    if(value == LIST_POISON){
        fprintf(file, "\t<td>PZ</td>");
    }
    else{
        fprintf(file, "\t<td>%d</td>", value);
    }

    return;
}

static void addPictureIntoLog(FILE* file, list_t* list){

    assert(file);
    assert(list);

    char pictureName[PICTURE_NAME_MAX_LENGTH] = "";
    sprintf(pictureName, "graph%u.png", ++(list->debugInfo.logPictureNum));

    createDebugImage(list, pictureName);

    fprintf(file, "<img src='%s' width=3000px>", pictureName);
}

static void printLinArr(FILE* file, const list_t* list){

    assert(list);
    assert(file);

    fprintf(file, "<p>\nПолный список в правильном порядке: ");

    int n = (int)list->head;
    if(n == 0) fprintf(file, "пусто\n");
    while(n != 0){
        fprintf(file, "%d, ", list->listArr[n].value);
        n = list->listArr[n].next;
    }
    fprintf(file, "</p>\n");
}

void launchLogFile(){

    char systemStartLogCommand[SYSTEM_COMMAND_MAX_LENGTH] = "";
    sprintf(systemStartLogCommand, "start \"\" \"%s\"", LOG_FILENAME);

    system(systemStartLogCommand);

    return;
}

ErrorCode dumpHeader(list_t* list){

    assert(list);

    FILE* file = list->logFilePtr;
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_FILE_NOT_FOUND;
    }

    char timeString[32] = {};                                        \
    getOperationTime(timeString, sizeof(timeString));

    fprintf(file, "<h3>DUMP [%s]-----------------------------</h3>\n", timeString);

    return ERR_OK;
}

ErrorCode loadDump(list_t* list){

    assert(list);

    FILE* file = list->logFilePtr;
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(file, "<h3>DUMP. List was loaded</h3>\n");

    return ERR_OK;
}

ErrorCode addDump(list_t* list, size_t indexAfter, list_elem_t elem){

    assert(list);

    FILE* file = list->logFilePtr;
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(file, "<h3>DUMP. Dump after ADD(%d) after index: %u</h3>\n", elem, indexAfter);

    return ERR_OK;
}

ErrorCode deleteDump(list_t* list, list_elem_t elem){

    assert(list);

    FILE* file = list->logFilePtr;
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(file, "<h3>DUMP. Dump after DELETE(%d)</h3>\n", elem);


    return ERR_OK;
}

ErrorCode openLogFile(FILE** file){

    *file = fopen(LOG_FILENAME, "a");
    if (*file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open log file: %s\n", LOG_FILENAME);

        return ERR_FILE_NOT_FOUND;
    }

    return ERR_OK;
}


#endif
