#include"GenericList.h"
#include<stdio.h>

#define MAX_NODES (5)

GenListError compare(void* dataIn, void* dataOut)
{
    return *(int*)dataIn == *(int*)dataOut ? GEN_LIST_MATCH : GEN_LIST_NO_MATCH;
}

int main(void)
{
    GenListError status = GEN_LIST_NO_ERR;
    GenList* list = NULL;
    unsigned int buf[MAX_NODES] = {1, 2, 3, 4, 5};
    unsigned int i = 0, size = 0, value = 0;

    printf("\nInitialize the list.");
    list = genListNew(sizeof(int), compare);
    if (!list) {
        printf("\ngenListNew failed.");
        return 1;
    }
    printf("\nPush %d nodes:", MAX_NODES);
    for (i = 0 ; i < MAX_NODES ; i++) {
        printf("\nPush value: %d.", buf[i]);
        status = genListPushHead(list, &buf[i]);
        if (status != GEN_LIST_NO_ERR) {
            printf("\ngenListPushHead() error: %d", (int)status);
            return 1;
        }
    }
    for (i = 0 ; i < MAX_NODES ; i++) {
        printf("\nGet value on index %d.", i);
        status = genListGetIndex(list, i, &value, sizeof(int));
        if (status != GEN_LIST_NO_ERR) {
            printf("\ngenListGetIndex() error: %d", (int)status);
            return 1;
        }
        printf("\nGot value: %d.", value);
    }
    status = genListGetSize(list, &size);
    if (status != GEN_LIST_NO_ERR) {
        printf("\ngenListGetSize() error: %d", (int)status);
        return 1;
    }
    printf("\nPushed %d nodes.", size);
    for (i = 0 ; i < MAX_NODES ; i++) {
        printf("\nFind value: %d.", buf[i]);
        status = genListSearchNode(list, &buf[i], &value, sizeof(int));
        if (status != GEN_LIST_NO_ERR) {
            printf("\ngenListSearchNode() error: %d", (int)status);
            return 1;
        }
        printf("\nFound value: %d.", value);
    }
    for (i = 0 ; i < MAX_NODES ; i++) {
        printf("\nPop a value from the list.");
        status = genListPopHead(list, &value, sizeof(int));
        if (status != GEN_LIST_NO_ERR) {
            printf("\ngenListPopHead() error: %d", (int)status);
            return 1;
        }
        printf("\nPoped value: %d.", value);
        status = genListGetSize(list, &size);
        if (status != GEN_LIST_NO_ERR) {
            printf("\ngenListGetSize() error: %d", (int)status);
            return 1;
        }
        printf("\nValues left: %d.", size);
    }
    printf("\nDestroy the list.");
    status = genListDestroy(list);
    if (status != GEN_LIST_NO_ERR) {
        printf("\ngenListDestroy() error: %d", (int)status);
        return 1;
    }
    printf("\nDone.");
    getchar();
}