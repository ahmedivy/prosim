#ifndef PRIO_Q_H
#define PRIO_Q_H

/* This is a singly linked-list implementation of a priority queue, nothing special
 * Items are kept in priority order where lower value is a higher priority.
 * I.e., the head of the queue has the lowest priority
 * Ties are broken by order of instertions into queue.
 * The priority queue stores pointers to the item and does not make a copy of the item
 * Instead of freeing nodes, the nodes are kept in a list to be reused.
 */

typedef struct node {
    struct node *next;    /* pointer to next node in the list */
    int priority;         /* priority of item in the queue */
    void *contents;       /* pointer to item */
} node_t;

typedef struct prio_q {
    node_t *head;         /* pointer to head node in list or null if empty */
    node_t *tail;         /* pointer to tail node in list of null if empty */
    node_t *free;         /* singly linked list of nodes that can be reused */
} prio_q_t;

/* Creates an empty priority queue and returns a pointer to it.
 * @params:
 *   none
 * @returns:
 *   pointer to the new priority queue or NULL if an error has occurred
 */
extern prio_q_t *prio_q_new();

/* Enqueues an item into the priority queue
 * @params:
 *   queue : pointer to the priority queue
 *   contents : pointer to item to be enqueued
 *   priority : item's priority
 * @returns:
 *   none
 */
extern void prio_q_add(prio_q_t *queue, void *contents, int priotity);

/* Removes and returns the item at the head of the queue.
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   pointer to the item or crashes if empty
 */
extern void *prio_q_remove(prio_q_t *queue);

/* Returns but does not remove the item at the head of the queue.
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   pointer to the item or crashes if empty
 */
extern void *prio_q_peek(prio_q_t *queue);

/* Returns true if the queue is empty
 * @params:
 *   queue : pointer to the priority queue
 * @returns:
 *   1 if queue is empty and 0 otherwise.
 */
extern int prio_q_empty(prio_q_t  *queue);

#endif //PRIO_Q_H
