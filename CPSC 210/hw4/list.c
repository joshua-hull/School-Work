// CP SC 210 hw4: list.c (c) 2012 Joshua Hull
// NOTE: This program is a prime example of why I always personally use doubly
// linked lists.

/* List functions */

#include "list.h"
#include <assert.h>

/** newList -- create a new list object **/
list_t *newList() {
    list_t *new;
    new = malloc(sizeof(list_t));
    new->head = NULL;
    new->tail = NULL;
    return new;
}


/** l_add -- add an object to the linked list **/
void l_add(list_t *list, void *objPtr) {
   node_t *new;
   /* Create new node */
   new = malloc(sizeof(node_t));
   new->objPtr = objPtr;
   new->next = NULL;

   /* Link new node onto end of list */
   if (list->tail != NULL) {
      /* Point node currently at end of list to this new node */
      list->tail->next = new;
   }
   else {
      /* List currently empty -- need to point head to new node */
      list->head = new;
   }
   /* Always point tail to new node */
   list->tail = new;
}

/** newIterator -- create a new iterator and associate it with the
                   specified list
**/
iterator_t *newIterator(list_t *list) {
   iterator_t *new;
   new = malloc(sizeof(iterator_t));
   new->list = list;
   new->position = list->head;
   new->lastRetrieved = NULL;
   return(new);
}

/** l_next -- return the object pointed to by the node pointed to by
            position, and then advance position to the next node in 
            the list.
**/
void *l_next(iterator_t *iter) {
    node_t *nodePtr = iter->position;
    if (nodePtr == NULL) {
       iter->lastRetrieved = NULL;
       return NULL;
    }

    /* Advance to next node in list */
    iter->lastRetrieved = nodePtr;
    iter->position = nodePtr->next;
    return (nodePtr->objPtr);
}

/** l_begin -- reset position to point to first node of list **/
void l_begin(iterator_t *iter) {
    iter->position = iter->list->head;
    iter->lastRetrieved = NULL;
}

/** l_hasnext -- test for end of list **/
int l_hasnext(iterator_t *iter) {
   if (iter->position != NULL) {
      return(1);
   }
   else {
      return(0);
   }
}

/** l_remove -- remove last node retrieved from list **/
void *l_remove(iterator_t *iter) {
    void *returnObject;
    node_t *ptr;
    node_t *previous;
    
    previous = NULL;
    ptr = iter->list->head;
    
    // If the iterator hasn't even retrieved the first element then we don't
    // even know if there are ANY elements in the list.
    if(iter->lastRetrieved == NULL) {
        return(NULL);
    }
    
    // If the node to remove is at the begining of a multi-node list...
    if((iter->lastRetrieved == iter->list->head && iter->list->head != iter->list->tail)||iter->lastRetrieved==NULL) {
        // Grab the object that the node to delete is pointing to.
        returnObject = ptr->objPtr;
        // Point the head of the list to the second node.
        iter->list->head = ptr->next;
        iter->lastRetrieved = NULL;
        // Free up the space that the deleted node took up.
        free(ptr);
        return(returnObject);
    // If the node to remove is the only node in the list...
    } else if( iter->lastRetrieved == iter->list->head && iter->list->head == iter->list->tail) {
        // Grab the object that the node to delete is pointing to.
        returnObject = ptr->objPtr;
        // Since we deleted the ONLY node set both head and tail to NULL
        iter->list->head = NULL;
        iter->list->tail = NULL;
        iter->lastRetrieved = NULL;
        // Free up some space
        free(ptr);
        return(returnObject);
    }
    // If we aren't dealing with the first node in the list then we need to 
    // find the node we want to delete while also keeping track to find the 
    // node behind the one we want to delete
    while (ptr != iter->lastRetrieved) {
        previous = ptr;
        ptr = ptr->next;
    }
    // If we are at the end of a multi-node list...
    if (ptr->next == NULL) {
        // Grab the object we want to return
        returnObject = ptr->objPtr;
        // Set the new tail to the node behind the current tail
        iter->list->tail = previous;
        // Have the new tail not have a next node
        previous->next = NULL;
        // and free up some more space.
        free(ptr);
        iter->lastRetrieved = NULL;
        return(returnObject);
    } else {
        // Grab the object we want to return
        returnObject = ptr->objPtr;
        // tell the node behind the one we are deleting to point to the node in
        // front of the one we are deleting.
        previous->next = ptr->next;
        iter->lastRetrieved = NULL;
        // Free up some space
        free(ptr);
        return(returnObject);
    }
}
