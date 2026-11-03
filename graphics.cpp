#ifdef LIST_DEBUG

static ErrorCode formPicture(list_t* list);
static void recordGraphicBlock(size_t num, listArrElem elem, FILE* file);
static void createGraphicConnections(FILE* file, list_t* list);
static void addEmptyArrow(FILE* file, size_t current, size_t next);
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
        printErrorIntoLog(list->logFilePtr, &debugLogInfo);
        printf("Cannot open graphic code file: %s\n", fileName);

        return ERR_FILE_NOT_FOUND;
    }

    fprintf(graphicCodeFile,
        "digraph{ bgcolor=\"#F0F7FF\"; rankdir=LR; \n");

    for(size_t i = 0; i < list->capacity; i++){

        recordGraphicBlock(i+1, list->listArr[i], graphicCodeFile);
        addEmptyArrow(graphicCodeFile, i, list->listArr[i].next);

    }

    createGraphicConnections(graphicCodeFile, list);
    recordOtherNodes(graphicCodeFile, list);

    fprintf(graphicCodeFile, "\n}\n");

    FCLOSE(graphicCodeFile);

    return ERR_OK;
}


static void recordGraphicBlock(size_t num, listArrElem elem, FILE* file){

    assert(file);

    if(num == 1){

        fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#aa62d7\"; color=\"#481569\";"
                    "fontcolor=\"#481569\"; "
                    "label = \"{{ <index> index = %u | value = PZ | <next> next = %d | <prev> prev = %d }}\";]\n",
                    num,
                    num-1,
                    elem.next,
                    elem.prev);
    }

    else if(elem.value == LIST_POISON){

        fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#e76f77\"; color=\"#940811\";"
                    "fontcolor=\"#940811\"; "
                    "label = \"{{ <index> index = %u | value = PZ | <next> next = %d | <prev> prev = %d }}\";]\n",
                    num,
                    num-1,
                    elem.next,
                    elem.prev);
    }
    else{

        fprintf(file, "\tnode%d [shape=Mrecord; style = filled; fillcolor=\"#2da8e1\"; color=\"#065e87\";"
                        "fontcolor=\"#065e87\"; "
                        "label = \"{{ <index> index = %u | value = %d | <next> next = %d | <prev> prev = %d }}\";]\n",
                        num,
                        num-1,
                        elem.value,
                        elem.next,
                        elem.prev);
    }
    return;
}

static void createGraphicConnections(FILE* file, list_t* list) {

    assert(file);
    assert(list);

    for (size_t i = 0; i < list->capacity; i++) {

        int next = list->listArr[i].next;
        int prev = list->listArr[i].prev;

        if (next > 0 && (size_t)next < list->capacity && list->listArr[next].prev == (int)i) {

            fprintf(file,
                    "\tnode%u:next -> node%d:prev "
                    "[dir=both, color=\"#5680E9\", "
                    "constraint=false, arrowsize=0.8];\n",
                    i + 1, next + 1);
        }

        if (prev == -1 && next > 0 && (size_t)next < list->capacity) {

            fprintf(file,
                    "\tnode%u:next -> node%d:index "
                    "[color=\"#b1105b\", constraint=false];\n",
                    i + 1, next + 1);
        }

        if (prev > 0 && (size_t)prev < list->capacity && list->listArr[prev].next != (int)i) {

            fprintf(file,
                "\tnode%u:prev:w -> node%d:index:e "
                "[color=\"#8860D0\", constraint=false];\n",
                i + 1, prev + 1);
            }
    }
}

static void addEmptyArrow(FILE* file, size_t current, size_t next) {

    assert(file);

    fprintf(file,
            "node%u -> node%d "
            "[style=invis, weight=1000];\n",
            current + 1, next + 1);
}

static void recordOtherNodes(FILE* file, list_t* list){

    assert(file);
    assert(list);

    fprintf(file, "\thead [shape=Mrecord; style = filled; fillcolor=\"#f5ad92\"; color=\"#00577f\";"
                        "fontcolor=\"#00577f\"; "
                        "label = \"head = %u\";]\n", list->head);

    fprintf(file, "\ttail [shape=Mrecord; style = filled; fillcolor=\"#f5ad92\"; color=\"#734a8d\";"
                        "fontcolor=\"#734a8d\"; "
                        "label = \"tail = %u\";]\n", list->tail);

    fprintf(file,
        "\tfree [shape=box; style=filled; fillcolor=\"#f5ad92\"; color=\"#b30510\"; "
        "fontcolor=\"#b30510\"; "
        "label=\"free = %u\"];\n",
        list->free);

    fprintf(file,
        "\thead -> node%u "
        "[color=\"#00577f\", constraint=true, weight=10];\n",
        list->head + 1);
    fprintf(file,
        "\ttail -> node%u "
        "[color=\"#452558\", constraint=true, weight=10];\n",
        list->tail + 1);

    fprintf(file,
            "\tfree -> node%u "
            "[color=\"#b30510\", constraint=true, weight=10];\n",
            list->free + 1);
    // fprintf(file, " { rank = same; node%u; head; }", list->head+1);
    // fprintf(file, " { rank = same; node%u; tail; }", list->tail+1);
    // fprintf(file, " { rank = same; node%u; free; }", list->free+1);
    }

#endif
