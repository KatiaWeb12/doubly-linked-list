#ifdef LIST_DEBUG

void createLogSpace(){

    char timeBuffer[DIRECTORY_NAME_MAX_LENGTH] = "";
    getOperationTime(timeBuffer, DIRECTORY_NAME_MAX_LENGTH);

    char directoryName[DIRECTORY_NAME_MAX_LENGTH] = "log_";
    strcat(directoryName, timeBuffer);

    char directoryCreationCommand[DIRECTORY_NAME_MAX_LENGTH] = "mkdir ";
    strcat(directoryCreationCommand, directoryName);

    system(directoryCreationCommand);
    _chdir(directoryName);
}

ErrorCode htmlListDump(const char* logFileName, list_t* list){

    assert(logFileName);
    assert(list);

    FILE* file = fopen(logFileName, "a");
    if (file == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open log file: %s\n", logFileName);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(file, "<link rel=\"stylesheet\" href=\"../styles.css\">");
    fprintf(file, "<pre>\n");

    printDebugInfoIntoLogFile(file, list);

    FCLOSE(file);

    return ERR_OK;
}

void printDebugInfoIntoLogFile(FILE* file, list_t* list){

    assert(list);
    assert(file);

    fprintf(file, "<p>\nlist_t '%s'[%p] created by %s() at %s:%u</p>",
            list->debugInfo.name,
            list,
            list->debugInfo.function,
            list->debugInfo.file,
            list->debugInfo.line);

    fprintf(file, "<p>capacity = %u</p>", list->capacity);
    fprintf(file, "<p>head = %u</p>", list->head);
    fprintf(file, "<p>tail = %u</p>", list->tail);
    fprintf(file, "<p>free = %u</p>", list->free);

    fprintf(file, "<p>listArr [%p]: </p>", list->listArr);

    fprintf(file, "<table border=\"1\">");

    printArr(file, list->listArr);

    fprintf(file, "</table>\n");

    addPictureIntoLog(file, list);

    printLinArr(file, list);

    return;
}

void printArr(FILE* file, listArrElem* arr){

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

void printArrEl(FILE* file, const list_elem_t value){

    assert(file);

    fprintf(file, "\t<td>%d</td>", value);

    return;
}

void addPictureIntoLog(FILE* file, list_t* list){

    assert(file);
    assert(list);

    char pictureName[PICTURE_NAME_MAX_LENGTH] = "";
    sprintf(pictureName, "graph%u.png", ++logPictureNum);

    createDebugImage(list, pictureName);

    fprintf(file, "<img src='%s' width=2000px>", pictureName);
}

void printLinArr(FILE* file, const list_t* list){

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

void launchLogFile(const char* logFileName){

    assert(logFileName);

    char systemStartLogCommand[SYSTEM_COMMAND_MAX_LENGTH] = "";
    sprintf(systemStartLogCommand, "start \"\" \"%s\"", logFileName);

    system(systemStartLogCommand);

    return;
}

#endif
