#include "heap.h"

heap* createHeap() {
    heap *h = malloc(sizeof(heap));
    h->head = malloc(sizeof(treeLevel));

    h->head->arraySize = 1;
    h->head->insertIndex = 0;
    h->head->data = calloc(h->head->arraySize, sizeof(node));
    h->head->parent = NULL;
    h->head->child = NULL;

    h->bottomLevel = h->head;

    return h;
}

int deleteHeap(heap* h) {
    treeLevel* cLevel = h->head;
    treeLevel* nLevel = cLevel->child;
    free(cLevel->data[0]);
    free(cLevel->data);
    free(cLevel);

    while (nLevel != NULL) {
        cLevel = nLevel;
        nLevel = cLevel->child;
        for(int i = 0; i < cLevel->arraySize; i++) {
            free(cLevel->data[i]);
        }
        free(cLevel->data);
        free(cLevel);
    }

    free(h);
    return 0;
}

int swap(treeLevel* tl1, int i1, treeLevel* tl2, int i2) {
    node* temp = tl1->data[i1];
    tl1->data[i1] = tl2->data[i2];
    tl2->data[i2] = temp;
    return 0;
}

int peek(heap* h) {
    return h->head->data[0]->key;
}

int push(heap* h, int key, void* data) {
    node* iNode = malloc(sizeof(node));
    iNode->key = key;
    iNode->data = data;
    h->bottomLevel->data[h->bottomLevel->insertIndex] = iNode;
    upHeap(h);
    h->bottomLevel->insertIndex++;
    if (h->bottomLevel->insertIndex == h->bottomLevel->arraySize) {
        treeLevel* iLevel = malloc(sizeof(treeLevel));
        iLevel->arraySize = h->bottomLevel->arraySize * 2;
        iLevel->insertIndex = 0;
        iLevel->data = calloc(iLevel->arraySize, sizeof(node));
        iLevel->parent = h->bottomLevel;
        iLevel->parent->child = iLevel;
        iLevel->child = NULL;
        h->bottomLevel = iLevel;
    }
    return 0;
}

int upHeap(heap* h) {
    treeLevel* cLevel = h->bottomLevel;
    int i = h->bottomLevel->insertIndex;
    if (cLevel->parent == NULL)
        return 0;

    do {
        if (cLevel->data[i]->key >= cLevel->parent->data[i/2]->key)
            return 0;

        swap(cLevel, i, cLevel->parent, i/2);
        cLevel = cLevel->parent;
        i /= 2;
    } while (cLevel->parent != NULL);
    return 0;
}

node* pop(heap* h) {
    node* rNode = h->head->data[0];
    h->head->data[0] = NULL;
    if (h->bottomLevel->insertIndex == 0 && h->bottomLevel != h->head) {
        h->bottomLevel = h->bottomLevel->parent;
        free(h->bottomLevel->child->data);
        free(h->bottomLevel->child);
        h->bottomLevel->child = NULL;
    }
    h->bottomLevel->insertIndex--;
    swap(h->head, 0, h->bottomLevel, h->bottomLevel->insertIndex);
    downHeap(h);
    return rNode;
}

int downHeap(heap* h) {
    treeLevel* cLevel = h->head;
    int i = 0;
    while (cLevel->child != NULL && cLevel->child->data[2*i] != NULL){
        if (cLevel->child->data[2*i + 1] != NULL) {
            if (cLevel->data[i]->key > cLevel->child->data[2*i]->key
                && cLevel->child->data[2*i]->key < cLevel->child->data[2*i+1]->key) {
                swap(cLevel, i, cLevel->child, 2*i);
                cLevel = cLevel->child;
                i = 2*i;
            } else if (cLevel->data[i]->key > cLevel->child->data[2*i+1]->key
                && cLevel->child->data[2*i + 1]->key < cLevel->child->data[2*i]->key) {
                swap(cLevel, i, cLevel->child, 2*i + 1);
                cLevel = cLevel->child;
                i = 2*i + 1;
            } else {
                return 0;
            }
        } else {
            if (cLevel->data[i]->key > cLevel->child->data[2*i]->key) {
                swap(cLevel, i, cLevel->child, 2*i);
                cLevel = cLevel->child;
                i = 2*i;
            } else {
                return 0;
            }
        }
    }
    return 0;
}

int printHeap(heap* h) {
    treeLevel *l = h->head;
    while (l != NULL) {
        for (int i = 0; i < l->arraySize; i++) {
            if(l->data[i] != NULL) {
                printf("[%d] ", l->data[i]->key);
            } else {
                printf("[*]");
            }
        }
        printf("\n");
        l = l->child;
    }
    return 0;
}