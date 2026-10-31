#ifdef LIST_DEBUG

ErrorCode createDebugImage(list_t* list, char* pictureName){

    assert(list);
    assert(pictureName);

    formPicture(list);

    char createPictureSystemCommand[SYSTEM_COMMAND_MAX_LENGTH] = "dot graph.txt -Tpng -o ";
    strcat(createPictureSystemCommand, pictureName);

    system(createPictureSystemCommand);

    deletingResultFile("graph.txt");

    return ERR_OK;

}

ErrorCode formPicture(list_t* list){

    assert(list);

    const char fileName[] = "graph.txt";

    FILE* graphicCodeFile = fopen(fileName, "a");
    if (graphicCodeFile == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open graphic code file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(graphicCodeFile, "digraph{ bgcolor=\"#F0F7FF\";\n");

    for(size_t i = 0; i < list->capacity; i++){
        recordGraphicBlock(i+1, list->listArr[i], graphicCodeFile);
    }

    fprintf(graphicCodeFile, "\n}\n");

    FCLOSE(graphicCodeFile);

    return ERR_OK;
}

void recordGraphicBlock(size_t num, listArrElem elem, FILE* file){

    assert(file);

    fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#84CEEB\"; color=\"#5680E9\"; fontcolor=\"#253A68\"; "
                    "label = \"{ next = %d | prev = %d | value = %d}\";]\n",
                    num,
                    elem.next,
                    elem.prev,
                    elem.value);

    return;
}

#endif
