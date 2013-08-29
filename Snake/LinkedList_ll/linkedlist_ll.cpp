/*
 * Singly-linked list
 * jdunn32
 * linklist_ll.cpp
 */

#include "mbed.h"
#include "linkedlist_ll.h"

/* Initialize pointers in LLRoot structure to NULL
 */
void initializeLL(LLRoot * master) {
    master->head = NULL;
    master->tail = NULL;
}

/* Allocate a node to the head of the Linked List
 */
LLNode * addToHeadLL(LLRoot * master, char x, char y, char z) {
    if(master == NULL)
        return NULL;

    LLNode * newHead = (LLNode *) malloc(sizeof(LLNode));
    if(newHead == NULL)         /* If malloc() returns NULL, we were unable to */
        return NULL;            /* allocate memory */

    newHead->x = x;
    newHead->y = y;
    newHead->z = z;
    newHead->next = master->head;   /* newHead will now be in front of the old LL head, */
    master->head = newHead;         /* so its next field will point to old LL head and  */
                                    /* the master node fields are updated */

    if(master->tail == NULL)        /* If the tail in master is NULL, we are adding to an  */
        master->tail = newHead;     /* empty List. Update tail field to point to only node */

    return newHead;
}

/* Allocate a node to the tail of the Linked List
 */
LLNode * addToTailLL(LLRoot * master, char x, char y, char z) {
    if(master == NULL)
        return NULL;

    LLNode * newNode = (LLNode *) malloc(sizeof(LLNode));
    if(newNode == NULL)
        return NULL;
        
    newNode->x = x;
    newNode->y = y;
    newNode->z = z;
    newNode->next = NULL;

    if(master->tail == NULL) {
        master->head = newNode;
    }else{
        master->tail->next = newNode;
    }
    master->tail = newNode;

    return newNode;
}

/* Remove node at the head of the Linked List
 */
void removeHeadLL(LLRoot * master) {
    LLNode * temp = NULL;

    if((master != NULL) && (master->head != NULL)) {
        temp = master->head->next;
        free(master->head);
        master->head = temp;

        if(master->head == master->tail)
            master->tail = NULL;
    }
}

/* Remove a tail from a Linked List
 */
void removeTailLL(LLRoot * master) {
    LLNode * node = NULL;
    LLNode * prev = NULL;

    if((master != NULL) && (master->head != NULL)) {
        if(master->head == master->tail) {      /* Handle case where only one node */
            free(master->head);                 /* exists in the LL */
            master->head = master->tail = NULL;
            return;
        }
        prev = master->head;
        node = master->head->next;

        while(node != master->tail) {
            node = node->next;
            prev = prev->next;
        }
        prev->next = NULL;
        free(node);
        master->tail = prev;
    }
}

/* Print our integer data from Linked List as signed decimal integers
 */
void printListLL(const LLRoot * master) {
    LLNode * node = master->head;       /* Begin at the head node */

    if(node == NULL)
        if(LLDEBUG) printf("[Empty Linked List]");

    if(LLDEBUG) printf("fruit: [x: %d][y: %d][level: %d]\r\n", master->fruit_x, master->fruit_y, master->fruit_z);
    if(LLDEBUG) printf("direction: %d\r\n", master->direction);
    //if(LLDEBUG) printf("length: %d\r\n", master->length);
    
    if(LLDEBUG) printf("head node: [%d %d %d]\r\n", master->head->x, master->head->y, master->head->z);
    while(node != NULL) {               /* If the node exists, pull the data from it */
        if(LLDEBUG) printf("[x: %d][y: %d][level: %d]\r\n", node->x, node->y, node->z); /* and continue down the LL */
        node = node->next;
    }
    if(LLDEBUG) printf("tail node: [%d %d %d]\r\n", master->tail->x, master->tail->y, master->tail->z);
    if(LLDEBUG) printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n\r\n");
}

/* Free memory allocated from Linked List function calls
 */
void freeListLL(LLRoot * master) {
    LLNode * node = master->head;
    LLNode * temp = NULL;

    while(node != NULL) {
        temp = node->next;
        free(node);
        node = temp;
    }
    master->head = NULL;
    master->tail = NULL;
}

/* EOF */
