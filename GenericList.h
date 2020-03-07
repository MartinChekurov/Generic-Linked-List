/*
*	Author: Martin Chekurov
*/

#ifndef GEN_LIST_H_
#define GEN_LIST_H_

#include <stdlib.h>

typedef enum {

    GEN_LIST_NO_ERR,
    GEN_LIST_ERR

}GenListErr;

typedef GenListErr (*Compare)(void*, void*);
typedef struct GenList_t GenList;

GenList*   genListNew       (size_t dataSize, Compare cmp);
GenListErr genListDestroy   (GenList* list);
GenListErr genListPushHead  (GenList* list, void* data);
GenListErr genListPopHead   (GenList* list, void* buf, size_t size);
GenListErr genListSearchNode(GenList* list, void* data, void* buf, size_t size);
GenListErr genListGetSize   (GenList* list, size_t *size);
GenListErr genListGetIndex  (GenList* list, size_t index, void* buf, size_t size);

#endif