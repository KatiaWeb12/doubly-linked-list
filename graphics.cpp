#ifdef LIST_DEBUG

static ErrorCode formPicture(list_t* list);
static void recordGraphicBlock(size_t num, listArrElem elem, FILE* file);
static void createGraphicConnections(FILE* file, list_t* list);
static void addEmptyArrow(FILE* file, list_t* list);
static void recordOtherNodes(FILE* file, list_t* list);


ErrorCode createDebugImage(list_t* list, char* pictureName){

    assert(list);
    assert(pictureName);

    ErrorCode error = formPicture(list);
    if (error != ERR_OK) return error;

    char createPictureSystemCommand[SYSTEM_COMMAND_MAX_LENGTH] = "dot graph.txt -Tpng -o ";
    strcat(createPictureSystemCommand, pictureName);

    system(createPictureSystemCommand);

    deletingResultFile("graph.txt");

    return ERR_OK;

}

static ErrorCode formPicture(list_t* list){

    assert(list);

    const char fileName[] = "graph.txt";

    FILE* graphicCodeFile = fopen(fileName, "w");
    if (graphicCodeFile == NULL) {
        error_debug debugLogInfo = {};
        ERROR_STRUCT_FORMAT(&debugLogInfo, __func__, __LINE__, "File-problem", ERR_FILE_NOT_FOUND);
        printErrorIntoConsole(&debugLogInfo);
        printf("Cannot open graphic code file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(graphicCodeFile, "digraph{ bgcolor=\"#F0F7FF\"; rankdir=LR; \n");

    for(size_t i = 0; i < list->capacity; i++){
        recordGraphicBlock(i+1, list->listArr[i], graphicCodeFile);
    }

    addEmptyArrow(graphicCodeFile, list);

    createGraphicConnections(graphicCodeFile, list);

    recordOtherNodes(graphicCodeFile, list);

    fprintf(graphicCodeFile, "\n}\n");

    FCLOSE(graphicCodeFile);

    return ERR_OK;
}

static void recordGraphicBlock(size_t num, listArrElem elem, FILE* file){

    assert(file);

    if(elem.value == LIST_POISON){

        fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#ee90c5\"; color=\"#97077f\";"
                    "fontcolor=\"#970751\"; "
                    "label = \"{{ <index> index = %u | value = PZ | <next> next = %d | <prev> prev = %d }}\";]\n",
                    num,
                    num-1,
                    elem.next,
                    elem.prev);
    }
    else{

        fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#84CEEB\"; color=\"#5680E9\";"
                        "fontcolor=\"#253A68\"; "
                        "label = \"{{ <index> index = %u | value = %d | <next> next = %d | <prev> prev = %d }}\";]\n",
                        num,
                        num-1,
                        elem.value,
                        elem.next,
                        elem.prev);
    }
    return;
}

static void createGraphicConnections(FILE* file, list_t* list){

    assert(list);

    for(size_t i = 0; i < list->capacity; i++){

        if(i != 0 && list->listArr[i].prev == -1) continue;

        int next = list->listArr[i].next;
        int prev = list->listArr[i].prev;

        if (next != 0) {
            fprintf(file, "\tnode%u:<next> -> node%d:<index> [color=\"#5680E9\"; constraint=false];\n",
            i + 1,
            next + 1);
        }

        if (prev != 0) {
            fprintf(file, "\tnode%u:<prev> -> node%d:<index> [color=\"#8860D0\"; constraint=false];\n",
            i + 1,
            prev + 1);
        }

    }
}

static void addEmptyArrow(FILE* file, list_t* list){

    for(size_t i = 0; i < list->capacity-1; i++)
        fprintf(file, "\tnode%u -> node%d [style=invis, weight=300];\n", i + 1, i + 2);

}

static void recordOtherNodes(FILE* file, list_t* list){

    assert(file);
    assert(list);

    fprintf(file, "\thead [shape=Mrecord; style = filled; fillcolor=\"#6c1463\"; color=\"#fcc4ce\";"
                        "fontcolor=\"#fcc4ce\"; "
                        "label = \"head = %u\";]\n", list->head);

    fprintf(file, "\ttail [shape=Mrecord; style = filled; fillcolor=\"#18858a\"; color=\"#fcc4ce\";"
                        "fontcolor=\"#fcc4ce\"; "
                        "label = \"tail = %u\";]\n", list->tail);

    fprintf(file, "\tfree [shape=Mrecord; style = filled; fillcolor=\"#b1105b\"; color=\"#fcc4ce\";"
                        "fontcolor=\"#fcc4ce\"; "
                        "label = \"free = %u\";]\n", list->free);

    fprintf(file, "\thead -> node%u:<index> [color=\"#5d0476\"; constraint=false];\n", list->head+1);
    fprintf(file, "\ttail -> node%u:<index> [color=\"#18858a\"; constraint=false];\n", list->tail+1);
    fprintf(file, "\tfree -> node%u:<index> [color=\"#b1105b\"; constraint=false];\n", list->free+1);
}

#endif
