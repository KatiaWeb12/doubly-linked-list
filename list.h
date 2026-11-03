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
const size_t PROGRAM_CAPACITY = 500;
const char LOG_FILENAME[] = "log.html";

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

enum ListVerifyLevel {
    VERIFY_OFF = 0,
    VERIFY_LIGHT = 1,
    VERIFY_MEDIUM = 2,
    VERIFY_FULL = 3
};

enum NumDump{
    BEFORE = 1,
    AFTER = 2
};

#ifdef LIST_DEBUG
struct debugList_t {
    const char* name;
    const char* file;
    const char* function;
    size_t line;
    size_t logPictureNum;

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
        ListVerifyLevel verifyLevel;
        FILE* logFilePtr;
    #endif
};

// Prototypes

ErrorCode listInit(list_t* list, const size_t listCapacity);

//AddElem
size_t listAddElem(list_t* list, const size_t indexAfter, const list_elem_t value);
size_t getFreeEl(list_t* list, ErrorCode* error);

//DelElem
ErrorCode listDelElem(list_t* list, const size_t indexDel);

//DestroyList
ErrorCode listDestroy(list_t* list);

#ifdef LIST_DEBUG

//Graphic
ErrorCode createDebugImage(list_t* list, char* pictureName);

// Debug
ErrorCode createLogSpace(list_t* list);
ErrorCode htmlListDump(list_t* list);
void launchLogFile();
ErrorCode loadDump(list_t* list);
ErrorCode addDump(list_t* list, size_t indexAfter, list_elem_t elem, NumDump num);
ErrorCode deleteDump(list_t* list, list_elem_t elem, NumDump num);
ErrorCode destroyDump(list_t* list);
ErrorCode openLogFile(FILE** file);
void printErrorIntoLog(FILE* logFile, struct error_debug* debugLogInfo);

// Verify
ListVerifyCode listVerify(list_t* list, const char* funcName, int line);
ErrorCode getVerifyLevel(int argc, char* argv[], list_t* list);

//Tests
ErrorCode doneTest(int argc, char* argv[]);
ErrorCode brokenTest(int argc, char* argv[]);
ErrorCode doneTestInsertBeforeHead(int argc, char* argv[]);

// Macro

#define ERROR_STRUCT_VERIFY_FORMAT(ptr, funcName, lineNum, codeValue)    \
    do {                                                                 \
        char timeString[32] = {};                                        \
        (ptr)->time = getOperationTime(timeString, sizeof(timeString));  \
        (ptr)->file = __FILE__;                                          \
        (ptr)->function = (funcName);                                    \
        (ptr)->line = (lineNum);                                         \
        (ptr)->errCode = (codeValue);                                    \
    } while (0)


#define VERIFY_OK(list, function)                                      \
    do {                                                               \
        if (listVerify((list), (function), __LINE__) != LIST_OK) {     \
            abort();                                                   \
        }                                                              \
    } while (0)                                                        \

#endif
