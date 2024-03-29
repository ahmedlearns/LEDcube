/*
 * Singly-linked list
 * jdunn32
 * linklist_ll.h
 */

#ifndef LINKEDLIST_LL_H_
    #define LINKEDLIST_LL_H_
    
#define LLDEBUG 0
#define XPOS 1
#define XNEG 2
#define YPOS 3
#define YNEG 4
#define ZPOS 5
#define ZNEG 6
#define BOUNDARY 7
#define CUBEDIM 8

/* Defined Structures for LL Snake holding LED cube data (per LED)*/
typedef struct llroot_ {
    char direction;
    //char length;
    char fruit_x;
    char fruit_y;
    char fruit_z;
    
    struct llnode_ * head;
    struct llnode_ * tail;
} LLRoot;

typedef struct llnode_ {
    signed char x, y, z;
    struct llnode_ * next;
} LLNode;

/* Function Prototypes */
LLNode * addToHeadLL(LLRoot * master, char x, char y, char z);
LLNode * addToTailLL(LLRoot * master, char x, char y, char z);

void removeHeadLL(LLRoot * master);
void removeTailLL(LLRoot * master);

void printListLL(const LLRoot * master);

void initializeLL(LLRoot * master);
void freeListLL(LLRoot * master);

#endif

/* EOF */
