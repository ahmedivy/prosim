#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "context.h"

static const char *OPS [] = {"HALT", "DOOP", "LOOP", "END", "BLOCK", NULL};


#define PUSH(s,v) (*(s++) = v)
#define POP(s) (*(--s))
#define PEEK(s) (*(s - 1))

/* Move the instruction pointer to the next DOOP, BLOCK or HALT to be executed.
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   1 if DOOP or BLOCK is the next primitive.
 *   0 if HALT is the next primitive
 *   -1 is returned if an unknown primitive is encountered.
 */
extern context *context_load(FILE *fin) {
    /* Allocate new context and assume that it is successful,
     */
    context *cur = calloc(1, sizeof(context));
    assert(cur);

    /* Read in the program description header and do some very basic validation
     * We assume it will be correct for the most part.
     */
    int size;
    if (fscanf(fin, "%10s %d %d %d", cur->name, &size, &cur->priority, &cur->thread) < 4) {
        fprintf(stderr, "Bad input: Expecting program name, size, priority, and thread\n");
        return NULL;
    }

    /* Allocate the primitive array and stack for the process.
     * We assume that the allocations will be successful.
     */
    cur->stack = malloc(2 * sizeof(int) * size);
    assert(cur->stack);

    cur->code = malloc(size * sizeof(opcode));
    assert(cur->code);

    /* ip = -1 because we assume that the next primitive to execute will be at index 0
     */
    cur->ip = -1;

    /* Read in the primitives with very basic validation
    */
    for (int i = 0; i < size; i++) {
        char op[10];

        /* Since not all primitives have an argument, we have to read in the primitive first
         */
        if (fscanf(fin, "%9s", op) < 1) {
            fprintf(stderr, "Bad input: Expecting operation on line %d in %s\n",
                    i + 1, cur->name);
            return NULL;
        }

        /* Assume that we will find hte opcode in the OPS array
         * This avoids needing a big IF statement.
         * We use an if statement to identify which primitives have an argument
         * Apart from checking that the argument is an integer, no validation is done.
         */
        cur->code[i].op = -1;
        for (int j = 0; OPS[j]; j++) {
            if (!strcmp(op, OPS[j])) {
                cur->code[i].op = j;
                if (j == OP_LOOP || j == OP_DOOP || j == OP_BLOCK) {
                    if (fscanf(fin, "%d", &cur->code[i].arg) < 1) {
                        fprintf(stderr, "Bad input: Expecting argument to op on line %d in %s\n",
                                i + 1, cur->name);
                        return NULL;
                    }
                }
                break;
            }
        }

        /* This is what happens if the Opcode is unknown.
         */
        if (cur->code[i].op == -1) {
            fprintf(stderr, "Bad input: operation %d unknown: %s\n", i + 1, op);
            return NULL;
        }
    }
    return cur;
}

/* Move the instruction pointer to the next DOOP, BLOCK or HALT to be executed and return the primitive.
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   1 if DOOP or BLOCK is the next primitive.
 *   0 if HALT is the next primitive
 *   -1 is returned if an unknown primitive is encountered.
 */
extern int context_next_op(context *cur) {
    int count;

    /* Move the IP along until a DOOP, BLOCK, or HALT is encountered.
     * LOOPs and ENDs are handled inside the loop.
     * Statistics are updated depending on the primitive.
     */
    for (;;) {
        cur->ip++;
        switch (cur->code[cur->ip].op) {
            case OP_LOOP:
                /* Use a stack to keep track of nested loops by pushing
                 * the start of loop and number of iterations on the stack.
                 */
                PUSH(cur->stack, cur->ip);
                PUSH(cur->stack, cur->code[cur->ip].arg);
                break;
            case OP_DOOP:
                cur->doop_count++;
                cur->doop_time += cur->code[cur->ip].arg;
                return 1;
            case OP_BLOCK:
                cur->block_count++;
                cur->block_time += cur->code[cur->ip].arg;
                return 1;
            case OP_END:
                /* The top of stack contains current loop info.
                 * Number of iterations is one-less now.
                 */
                count = POP(cur->stack);
                count--;
                if (count == 0) {
                    /* Stack needs to be cleared if the loop is done.
                     */
                    POP(cur->stack);
                } else {
                    /* Stack needs to be updated with new count and
                     * ip moved to start of loop body.
                     */
                    cur->ip = PEEK(cur->stack);
                    PUSH(cur->stack, count);
                }
                break;
            case OP_HALT:
                return 0;
            default:
                printf("error, unknown opcode %d at ip %d\n", cur->code[cur->ip].op, cur->ip);
                return -1;
        }
    }
}

/* returns the duration of the current primitive.
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   number of clock ticks of the current primitive,
 */
extern int context_cur_duration(context *cur) {
    assert(cur->ip >= 0);
    return cur->code[cur->ip].arg;
}

/* Returns the current primitive being executed
 * @params:
 *   cur: pointer to process context
 * @returns:
 *   the primitive being executed: one of OP_HALT, OP_DOOP, or OP_BLOCK.
 */
extern int context_cur_op(context *cur) {
    assert(cur->ip >= 0);
    return cur->code[cur->ip].op;
}

/* Outputs aggregate statistics about a process to the specified file.
 * @params:
 *   cur: pointer to process context
 *   fout: FILE into which the output should be written
 * @returns:
 *   none
 */
extern void context_stats(context *cur, FILE *fout) {
    fprintf(fout,"| %5.5d | Proc %2.2d.%2.2d | Run %d, Block %d, Wait %d\n",
            cur->finished, cur->thread, cur->id, cur->doop_time, cur->block_time, cur->wait_time);
}

