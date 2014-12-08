#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* node in doubly linked list */
typedef struct dnode_type {
   char *word;                /* Pointer to word              */
   int  count;                /* Occurence count              */
   struct dnode_type *next;   /* Next node                    */
   struct dnode_type *prev;   /* Predecessor node             */
} dnode_t;

/* Doubly linked list structure */
typedef struct dlist_type {
   dnode_t *head;             /* Pointer to dummy header node */
   dnode_t *tail;             /* Pointer to dummy tail node   */
} dlist_t;

/* Prototype statements */
dlist_t *newDlist();
void addword(dlist_t *list, char *word);
void print(dlist_t *list);
void rprint(dlist_t *list);
int  deleteword(dlist_t *list, char *word);

#endif
