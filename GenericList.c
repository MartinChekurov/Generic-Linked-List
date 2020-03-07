#include "GenericList.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct GenListNode_t {

    void* data;
    struct GenListNode_t* pNext;

}GenListNode;

struct GenList_t {

    GenListNode* head;
    Compare cmp;
    size_t dataSize;
    size_t count;
};

static GenListErr genListCopyNodeData(void* data, size_t dataSize, void* buf, size_t bufSize)
{
    if (!data || !buf) {
        return GEN_LIST_ERR;
    }
    memcpy(buf, data, bufSize < dataSize ? bufSize : dataSize);
    return GEN_LIST_NO_ERR;
}

static GenListNode* genLisNewNode(void* data, size_t dataSize)
{
    if (!data) {
        return NULL;
    }
    GenListNode* node = malloc(sizeof(*node));
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

static GenListErr genListDestroyNode(GenListNode* node)
{
    if (!node) {
        return GEN_LIST_ERR;
    }
    free(node->data);
    free(node);
    return GEN_LIST_NO_ERR;
}

GenList* genListNew(size_t dataSize, Compare cmp)
{
    GenList* list = malloc(sizeof(*list));
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

GenListErr genListPushHead(GenList* list, void* data)
{
    if (!list || !data) {
        return GEN_LIST_ERR;
    }
    if (list->count == SIZE_MAX) {
        return GEN_LIST_ERR;
    }
    GenListNode* node = genLisNewNode(data, list->dataSize);
    if (!node) {
        return GEN_LIST_ERR;
    }
    node->pNext = list->head;
    list->head = node;
    list->count++;
    return GEN_LIST_NO_ERR;
}

GenListErr genListPopHead(GenList* list, void* buf, size_t size)
{
    GenListNode* node = NULL;
    GenListErr status = GEN_LIST_NO_ERR;
    if (!list) {
        return GEN_LIST_ERR;
    }
    if (list->head) {
        status = genListCopyNodeData(list->head->data, list->dataSize, buf, size);
        if (status != GEN_LIST_NO_ERR) {
            return status;
        }
        node = list->head;
        list->head = node->pNext;
        genListDestroyNode(node);
        if (list->count)
            list->count--;
    }
    return GEN_LIST_NO_ERR;
}

GenListErr genListGetIndex(GenList* list, size_t index, void* buf, size_t size)
{
    GenListNode* node = NULL;
    size_t count = 0;
    if (!list) {
        return GEN_LIST_ERR;
    }
    node = list->head;
    while(node && count < list->count) {
        if (count == index) {
            return genListCopyNodeData(node->data, list->dataSize, buf, size);
        }
        node = node->pNext;
        count++;
    }
    return GEN_LIST_ERR;
}

GenListErr genListSearchNode(GenList* list, void* data, size_t *index)
{
    GenListNode* node = NULL;
    size_t count = 0;
    if (!list || !data) {
        return GEN_LIST_ERR;
    }
    node = list->head;
    while(node) {
        if (list->cmp) {
            if (list->cmp(node->data, data) == GEN_LIST_NO_ERR) {
                if (index){
                    *index = count;
                }
                return GEN_LIST_NO_ERR;
            }
        }
        node = node->pNext;
        count++;
    }
    return GEN_LIST_ERR;
}

GenListErr genListDestroy(GenList* list)
{
    GenListNode* node = NULL;
    GenListErr status = GEN_LIST_NO_ERR;
    if (!list) {
        return GEN_LIST_ERR;
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

GenListErr genListGetSize(GenList* list, size_t *size)
{
    if (!list || !size) {
        return GEN_LIST_ERR;
    }
    *size = list->count;
    return GEN_LIST_NO_ERR;
}
