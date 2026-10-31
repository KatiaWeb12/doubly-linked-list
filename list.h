#include <TXLib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>

#include "../universal-features/error.h"
#include "../universal-features/colors.h"
#include "../universal-features/useful-functions.h"


#define LIST_POISON 0
#define PRINT_ELEM_FORMAT "%d"

const size_t LIST_CAPACITY = 15;
const size_t DIRECTORY_NAME_MAX_LENGTH = 50;
const size_t PICTURE_NAME_MAX_LENGTH = 50;
const size_t SYSTEM_COMMAND_MAX_LENGTH = 30;
const size_t PROGRAM_CAPACITY = 500; // Можно ли?
typedef int list_elem_t;

size_t logPictureNum = 0;

#ifdef LIST_DEBUG
struct debugList_t {
    const char* name;
    const char* file;
    const char* function;
    size_t line;
};
#endif

struct listArrElem {

    list_elem_t value;
    list_elem_t next;
    list_elem_t prev;
};

struct list_t {

    size_t capacity;
    listArrElem* listArr;

    size_t head;
    size_t tail;
    size_t free;

    #ifdef LIST_DEBUG
        debugList_t debugInfo;
    #endif
};

// Prototypes

ErrorCode listLoad(list_t* list, const size_t listCapacity);

//AddElem
ErrorCode listAddElem(list_t* list, const size_t indexAfter, const list_elem_t value);
size_t getFreeEl(list_t* list, ErrorCode* error);

//DelElem
ErrorCode listDelElem(list_t* list, const size_t indexDel);

#ifdef LIST_DEBUG

//Graphic
ErrorCode createDebugImage(list_t* list, char* pictureName);
ErrorCode formPicture(list_t* list);
void recordGraphicBlock(size_t num, listArrElem elem, FILE* file);

// Debug

ErrorCode listDump(const list_t* list);
void printArr(FILE* file, listArrElem* arr);
void printArrEl(FILE* file, const list_elem_t value);
void printLinArr(FILE* file, const list_t* list);
void printDebugInfoIntoLogFile(FILE* file, list_t* list);
ErrorCode htmlListDump(const char* logFileName, list_t* list);
void createLogSpace();
void addPictureIntoLog(FILE* file, list_t* list);
void launchLogFile(const char* logFileName);
#endif
