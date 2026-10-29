#define LIST_POISON 0
#define PRINT_ELEM_FORMAT "%d"

const size_t LIST_CAPACITY = 15;
typedef int list_elem_t;

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

ErrorCode listLoad(struct list_t* list, const size_t listCapacity);

// Debug
ErrorCode listDump(const list_t* list);
void printIndexes(listArrElem* arr);
void printArr(listArrElem* arr);
void printArrEl(list_elem_t value);
