/*
*	Author: Martin Chekurov
*/

#ifndef GEN_LIST_H_
#define GEN_LIST_H_

#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum {

    GEN_LIST_NO_ERR,
    GEN_LIST_ERR

}GenListErr;

typedef GenListErr (*Compare)(void*, void*);
typedef struct GenList_t GenList;

GenList*   genListNew       (unsigned int dataSize, Compare cmp);
GenListErr genListDestroy   (GenList* list);
GenListErr genListPushHead  (GenList* list, void* data);
GenListErr genListPopHead   (GenList* list, void* buf, unsigned int size);
GenListErr genListSearchNode(GenList* list, void* data, void* buf, unsigned int size);
GenListErr genListGetSize   (GenList* list, unsigned int *size);
GenListErr genListGetIndex  (GenList* list, unsigned int index, void* buf, unsigned int size);

#endif