#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prio_q.h"

/* Creates an empty priority queue and returns a pointer to it.
 * @params:
 *   none
 * @returns:
 *   pointer to the new priority queue or NULL if an error has occurred
 */
extern prio_q_t *prio_q_new() {
    prio_q_t * list = calloc(1, sizeof(prio_q_t));
    assert(list != NULL);
    return list;
}

/* Allocate a new node to be inserted into queue and initialize it.
 * @params:
 *   queue : pointer to the priority queue
 *   contents : pointer to item to be enqueued
 *   priority : item's priority
 * @returns:
 *   pointer to new node.
 */
static node_t *new_node(prio_q_t *queue, void *contents, int priority) {
    assert(queue != NULL);

    /* If our free list has free nodes, use one of them
     * Otherwise, allocate a new mpde.
     */
    node_t * node = queue->free;
    if (!node) {
        node = calloc(1, sizeof(node_t));
    } else {
        queue->free = queue->free->next;
        memset(node, 0, sizeof(node_t));
    }

    /* Assume we have a node and initialize it
     */
    assert(node != NULL);
    node->contents = contents;
    node->priority = priority;
    return node;
}

/* Enqueues an item into the priority queue
 * @params:
 *   queue : pointer to the priority queue
 *   contents : pointer to item to be enqueued
 *   priority : item's priority
 * @returns:
 *   none
 */
extern void prio_q_add(prio_q_t *list, void *contents, int priority) {
    /* Assume we successfully allocate a new node
     */
    node_t *node = new_node(list, contents, priority);

    /* Deal with four cases
     * 1. queue is empty
     * 2. new item is inserted at back of queue (priority >= tail->priority)
     * 3. new item is inserted at front of queue (priority < head->priority)
     * 4. new item is inserted in middle of queue (head->priority <= priority < tail->priority)
     */
    if (list->head == NULL) {
        list->tail = node;
        list->head = node;
    } else if (list->tail->priority <= priority) {
        list->tail->next = node;
        list->tail = node;
    } else if (list->head->priority > priority) {
        node->next = list->head;
        list->head = node;
    } else {
        /* If case 4, need to walk the list to find where to insert
         */
        node_t *tmp;
        for (tmp = list->head; tmp->next->priority <= priority; tmp = tmp->next );
        node->next = tmp->next;
        tmp->next = node;
    }
}

/* Returns true if the queue is empty
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   1 if queue is empty and 0 otherwise.
 */
extern int prio_q_empty(prio_q_t  *list) {
    assert(list != NULL);
    return list->head == NULL;
}

/* Removes and returns the item at the head of the queue.
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   pointer to the item or crashes if empty
 */
extern void * prio_q_remove(prio_q_t *list) {
    assert(list != NULL);
    assert(list->head != NULL);

    node_t *node = list->head;
    list->head = list->head->next;
    if (list->head == NULL) {
        /* if the queue becomes empty, be sure head and tail are NULL
         */
        list->tail = NULL;
    }

    /* instead of freeing the node, add it to the free list
     */
    node->next = list->free;
    list->free = node;
    return node->contents;
}

/* Returns but does not remove the item at the head of the queue.
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   pointer to the item or crashes if empty
 */
extern void * prio_q_peek(prio_q_t *list) {
    assert(list != NULL);
    assert(list->head != NULL);

    return list->head->contents;
}