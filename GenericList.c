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

static GenListError genListCopyNodeData(void* data, unsigned int dataSize, void* buf, unsigned int bufSize)
{
    unsigned int cSize = 0;
    if (!data || !buf) {
        return GEN_LIST_WRONG_PAR;
    }
    cSize = bufSize < dataSize ? bufSize : dataSize;
    memcpy(buf, data, cSize);
    return GEN_LIST_NO_ERR;
}

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
    memset(node, 0, sizeof(*node));
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

GenList* genListNew(unsigned int dataSize, Compare cmp)
{
    GenList* list = NULL;
    list = malloc(sizeof(*list));
    if (!list) {
        return NULL;
    }
    memset(list, 0, sizeof(*list));
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

GenListError genListPopHead(GenList* list, void* buf, unsigned int size)
{
    GenListNode* node = NULL;
    GenListError status = GEN_LIST_NO_ERR;
    if (!list) {
        return GEN_LIST_WRONG_PAR;
    }
    if (list->head) {
        node = list->head;
        list->head = node->pNext;
        status = genListCopyNodeData(node->data, list->dataSize, buf, size);
        if (status != GEN_LIST_NO_ERR) {
            return status;
        }
        genListDestroyNode(node);
        if (list->count)
            list->count--;
    }
    return GEN_LIST_NO_ERR;
}

GenListError genListGetIndex(GenList* list, unsigned int index, void* buf, unsigned int size)
{
    GenListNode* node = NULL;
    unsigned int count = 0;
    if (!list) {
        return GEN_LIST_WRONG_PAR;
    }
    node = list->head;
    while(node && count < list->count) {
        if (count == index) {
            return genListCopyNodeData(node->data, list->dataSize, buf, size);
        }
        node = node->pNext;
        count++;
    }
    return GEN_LIST_ERROR;
}

GenListError genListSearchNode(GenList* list, void* data, void* buf, unsigned int size)
{
    GenListNode* node = NULL;
    GenListError status = GEN_LIST_NO_ERR;
    if (!list || !data || !buf) {
        return GEN_LIST_WRONG_PAR;
    }
    node = list->head;
    while(node) {
        if (list->cmp) {
            status = list->cmp(node->data, data);
            if (status == GEN_LIST_MATCH) {
                return genListCopyNodeData(node->data, list->dataSize, buf, size);
            }
        }
        node = node->pNext;
    }
    return GEN_LIST_ERROR;
}

GenListError genListDestroy(GenList* list)
{
    GenListNode* node = NULL;
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
