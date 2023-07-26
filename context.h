#ifndef ASSIGNMENT_1_CONTEXT_H
#define ASSIGNMENT_1_CONTEXT_H

#include <stdio.h>
enum {
    OP_HALT, OP_DOOP, OP_LOOP, OP_END, OP_BLOCK, OP_LAST
};

typedef struct opcode {
    int op;                     /* primitive op code (see enum above) */
    int arg;                    /* argument value associated with the op code */
} opcode;

typedef struct context {
    opcode *code;               /* array of primitives */
    int *stack;                 /* stack for processing loops */
    char name[11];              /* program name */
    int ip;                     /* index of current primitive being executed */
    int id;                     /* process id */
    int priority;               /* process priority */
    int duration;               /* amount of clock ticks left in current primitive */
    int state;                  /* current state of process: NEW, READY, RUNNING, BLOCKED, FINISHED */
    int enqueue_time;           /* time at which process was added to ready queue */
    int doop_count;             /* number of DOOPs performed */
    int doop_time;              /* number of clock ticks spent executing DOOPs*/
    int block_count;            /* number of BLOCKs performed */
    int block_time;             /* number of clock ticks spent being blocked */
    int wait_count;             /* number of times process is added to the ready queue */
    int wait_time;              /* number of clock ticks spent waiting in ready queue */
    int thread;                 /* node id to which process is to be assigned */
    int finished;               /* time process finished */
} context;

/* Move the instruction pointer to the next DOOP, BLOCK or HALT to be executed.
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   1 if DOOP or BLOCK is the next primitive.
 *   0 if HALT is the next primitive
 *   -1 is returned if an unknown primitive is encountered.
 */
extern int context_next_op(context *cur);

/* Reads in a program description from a file and creates a context for it.
 * @params:
 *   fin: FILE from which to read
 * @returns:
 *   pointer to the new context or NULL if an error has occurred
 */
extern context *context_load(FILE *fin);

/* Outputs aggregate statistics about a process to the specified file.
 * @params:
 *   cur: pointer to process context
 *   fout: FILE into which the output should be written
 * @returns:
 *   none
 */
extern void context_stats(context *cur, FILE *fout);

/* returns the duration of the current primitive.
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   number of clock ticks of the current primitive,
 */
extern int context_cur_duration(context *cur);

/* Returns the current primitive being executed
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   the primitive being executed: one of OP_HALT, OP_DOOP, or OP_BLOCK.
 */
extern int context_cur_op(context *cur);


#endif //ASSIGNMENT_1_CONTEXT_H
