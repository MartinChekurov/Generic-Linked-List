#include "GenericList.h"

typedef struct GenListNode_t {

    void* data;
    struct GenListNode_t* pNext;

}GenListNode;

struct GenList_t {

    GenListNode* head;
    Compare cmp;
    unsigned int dataSize;
    unsigned int count;
};

static GenListNode* genLisNewNode(void* data, unsigned int dataSize)
{
    GenListNode* node = NULL;
    if (!data) {
        return NULL;
    }
    node = malloc(sizeof(*node));
    if (!node) {
        return NULL;
    }
    memset(node, 0, sizeof(node));
    node->data = malloc(dataSize);
    if (!node->data) {
        return NULL;
    }
    memcpy(node->data, data, dataSize);
    return node;
}

static GenListError genListDestroyNode(GenListNode* node)
{
    if (!node) {
        return GEN_LIST_WRONG_PAR;
    }
    free(node->data);
    free(node);
    return GEN_LIST_NO_ERR;
}

GenList* genLisNew(unsigned int dataSize, Compare cmp)
{
    GenList* list = NULL;
    list = malloc(sizeof(*list));
    if (!list) {
        return NULL;
    }
    memset(list, 0, sizeof(list));
    list->dataSize = dataSize;
    if (cmp) {
        list->cmp = cmp;
    }
    return list;
}

GenListError genListPushHead(GenList* list, void* data)
{
    GenListNode* node = NULL;
    if (!list || !data) {
        return GEN_LIST_WRONG_PAR;
    }
    if (list->count == UINT_MAX) {
        return GEN_LIST_FULL;
    }
    node = genLisNewNode(data, list->dataSize);
    if (!node) {
        return GEN_LIST_NO_MEMORY;
    }
    node->pNext = list->head;
    list->head = node;
    list->count++;
    return GEN_LIST_NO_ERR;
}

void* genListPopHead(GenList* list)
{
    GenListNode* node = NULL;
    void* data = NULL;
    if (!list) {
        return NULL;
    }
    if (list->head) {
        node = list->head;
        data = node->data;
        list->head = node->pNext;
        genListDestroyNode(node);
        if (list->count)
            list->count--;
    }
    return data;
}

void* genListGetIndex(GenList* list, unsigned int index)
{
    GenListNode* node = NULL;
    unsigned int count = 0;
    if (!list) {
        return NULL;
    }
    node = list->head;
    while(node && count < list->count) {
        if (count == index) {
            return node->data;
        }
        node = node->pNext;
        count++;
    }
    return NULL;
}

void* genListSearchNode(GenList* list, void* data)
{
    GenListNode* node = NULL;
    GenListError status = GEN_LIST_NO_ERR;
    if (!list || !data) {
        return NULL;
    }
    node = list->head;
    while(node) {
        if (list->cmp) {
            status = list->cmp(node->data, data);
            if (status == GEN_LIST_MATCH) {
                return node->data;
            }
        }
        node = node->pNext;
    }
    return NULL;
}

GenListError genListDestroy(GenList* list)
{
    GenListNode* node = NULL, *next = NULL;
    GenListError status = GEN_LIST_NO_ERR;
    if (!list) {
        return GEN_LIST_WRONG_PAR;
    }
    while(list->head) {
        node = list->head;
        list->head = node->pNext;
        status = genListDestroyNode(node);
        if (status != GEN_LIST_NO_ERR) {
            return status;
        }
    }
    free(list);
    return GEN_LIST_NO_ERR;
}

GenListError genListGetSize(GenList* list, unsigned int *size)
 {
    if (!list || !size) {
        return GEN_LIST_WRONG_PAR;
    }
    *size = list->count;
    return GEN_LIST_NO_ERR;
}