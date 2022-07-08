#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    
    /*  hint:
     *  1. if (count==0), insert `e` into `env_sched_list[1-point]`
     *     using LIST_REMOVE and LIST_INSERT_TAIL.
     *  2. if (env_sched_list[point] is empty), point = 1 - point;
     *     then search through `env_sched_list[point]` for a runnable env `e`, 
     *     and set count = e->env_pri
     *  3. count--
     *  4. env_run()
     *
     *  functions or macros below may be used (not all):
     *  LIST_INSERT_TAIL, LIST_REMOVE, LIST_FIRST, LIST_EMPTY
     */
	struct Env *e = curenv;
	struct Tcb *t = curtcb;
	// printf("before yield, thread is %x\n", t->tcb_id);
	if (t == NULL || t->tcb_status != ENV_RUNNABLE) {
		while (LIST_EMPTY(&tcb_sched_list[point])) {
			point = 1 - point;
		}
		t = LIST_FIRST(&tcb_sched_list[point]);
		// printf("in part1, thread is %x\n", t->tcb_id);
		count = t->tcb_pri;
		count--;
		env_run(t);
	}
	if (count <= 0) {
		LIST_REMOVE(t, tcb_sched_link);
		LIST_INSERT_HEAD(&tcb_sched_list[1 - point], t, tcb_sched_link);
		while (LIST_EMPTY(&tcb_sched_list[point])) {
			point = 1 - point;
		}
		t = LIST_FIRST(&tcb_sched_list[point]);
		// printf("in part2, thread is %x\n", t->tcb_id);
		count = t->tcb_pri;
	}
	count--;
	env_run(t);
}
