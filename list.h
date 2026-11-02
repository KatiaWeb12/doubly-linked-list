#include <TXLib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>

#include "../universal-features/error.h"
#include "../universal-features/colors.h"
#include "../universal-features/useful-functions.h"


#define LIST_POISON -56925
#define PRINT_ELEM_FORMAT "%d"

const size_t LIST_CAPACITY = 15;
const size_t DIRECTORY_NAME_MAX_LENGTH = 50;
const size_t PICTURE_NAME_MAX_LENGTH = 50;
const size_t SYSTEM_COMMAND_MAX_LENGTH = 50;
const size_t PROGRAM_CAPACITY = 500; // Можно ли?

typedef int list_elem_t;

enum ListVerifyCode {
    LIST_OK = 0,
    LIST_NULL = 1,
    LIST_NULL_ARRAY = 2,
    LIST_INVALID_CAPACITY = 3,
    LIST_INVALID_HEAD = 4,
    LIST_INVALID_TAIL = 5,
    LIST_INVALID_FREE = 6,
    LIST_INVALID_NEXT = 7,
    LIST_INVALID_PREV = 8,
    LIST_BAD_NEXT_PREV = 9,
    LIST_BAD_PREV_NEXT = 10,
    LIST_CYCLE = 11,
    LIST_FREE_CYCLE = 12,
    LIST_FREE_PREV = 13,
    LIST_LOST_ELEMENT = 14,
    LIST_ACTIVE_POISON = 15,
    LIST_INVALID_EXTRA_EL = 16
};


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
void createGraphicConnections(FILE* file, list_t* list);
void addEmptyArrow(FILE* file, list_t* list);
void recordOtherNodes(FILE* file, list_t* list);

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
ErrorCode loadDump(const char* logFileName);
ErrorCode addDump(const char* logFileName, size_t indexAfter, list_elem_t elem);
ErrorCode deleteDump(const char* logFileName, list_elem_t elem);

// Verify
ListVerifyCode listVerify(const char* logFileName, const list_t* list);
void printVerifyErrorIntoConsole(struct error_debug_verify* debugLogInfo);


#define ERROR_STRUCT_VERIFY_FORMAT(ptr, funcName, lineNum, codeValue) \
    do {                                                                 \
        char timeString[32] = {};                                        \
        (ptr)->time = getOperationTime(timeString, sizeof(timeString));  \
        (ptr)->file = __FILE__;                                          \
        (ptr)->function = (funcName);                                    \
        (ptr)->line = (lineNum);                                         \
        (ptr)->errCode = (codeValue);                                    \
    } while (0)

#endif
