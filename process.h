#ifndef PROSIM_PROCESS_H
#define PROSIM_PROCESS_H
#include "prio_q.h"
#include "context.h"


typedef struct processor {
    prio_q_t *blocked;       /* queue for blocked processes on node */
    prio_q_t *ready;         /* queue for blocked processes on node */
    int clock_time;          /* local node time */
    int next_proc_id;        /* local node process counter */
} processor_t;

/* Initialize the simulation
 * @params:
 *   quantum: the CPU quantum to use in the situation
 * @returns:
 *   returns 1
 */
extern void process_init(int cpu_quantum);

/* Create a new node context
 * @params:
 *   None
 * @returns:
 *   pointer to new node context.
 */
extern processor_t *process_new();

/* Admit a process into the simulation
 * @params:
 *   proc: pointer to the program context of the process to be admitted
 *   cpu : node context
 * @returns:
 *   returns 1
 */
extern int process_admit(processor_t *cpu, context *proc);

/* Perform the simulation
 * @params:
 *   cpu : node context
 * @returns:
 *   returns 1
 */
extern int process_simulate(processor_t *cpu);

/* Output process summary post execution
 * @params:
 *   fout : output file
 * @returns:
 *   none
 */
extern void process_summary(FILE *fout);

#endif //PROSIM_PROCESS_H
