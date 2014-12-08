// Lab 5: list.c (c) 2012 Joshua Hull

#include "list.h"

/** newList -- create a new list object **/
list_t *newList() {
    list_t *returnList;
    // Allocate a new list structure and initialize the values to the
    // appropriate values.
    returnList = (list_t *)malloc(sizeof(list_t));
    returnList->head = NULL;
    returnList->tail = NULL;
    return returnList;
}


/** l_add -- add an object to the linked list **/
void l_add(list_t *list, void *objPtr) {
    node_t *newNode;
    // Allocate a new node.
    newNode = (node_t *)malloc(sizeof(node_t));
    // If we have no Head and no Tail then we are the first element in the list
    if(list->head == NULL && list->tail == NULL){
        newNode->objPtr = objPtr;
        newNode->next = NULL;
        list->head = newNode;
        list->tail = newNode;
    }
    // Otherwise we are just adding to the end of the list
    else {
        newNode->objPtr = objPtr;
        newNode->next = NULL;
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

/** newIterator -- create a new iterator and associate it with the
                   specified list
**/
iterator_t *newIterator(list_t *list) {
    iterator_t *returnIterator;
    // Allocate a new iterator and initialize the values appropriatly
    returnIterator = (iterator_t *)malloc(sizeof(iterator_t));
    returnIterator->list = list;
    returnIterator->position = list->head;
    return returnIterator;
}

/** l_next -- return the object pointed to by the node pointed to by
            position, and then advance position to the next node in 
            the list.
**/
void *l_next(iterator_t *iter) {
    node_t *workingNode;
    // Create a tmp working copy.
    workingNode = iter->position;
    // Check to to see if we aren't past the end of the list
    if(iter->position != NULL) {
        // Move the position to the next position
        iter->position = iter->position->next;
        // Return the objPtr of the position we were handed.
        return workingNode->objPtr;
    } else {
        // We were at the end of the list and we should return NULL
        return NULL;
    }
}

/** l_begin -- reset position to point to first node of list **/
void l_begin(iterator_t *iter) {
    // Just move back to the head of the list
    iter->position = iter->list->head;
}

/** l_hasnext -- test for end of list **/
int l_hasnext(iterator_t *iter) {
    // If were are at position NULL then we have fallen off the end of the list
    // and we have no more elements.
    if (iter->position == NULL) {
        return(0);
    // Otherwise ther are more elements.
    } else {
        return(1);
    }
}

