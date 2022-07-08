#include "lib.h"
#include <error.h>
#include <mmu.h>

int pthread_create(pthread_t * thread, const pthread_attr_t * attr, void * (*start_routine)(void *), void *arg) {
    int newthread = syscall_thread_alloc();
	if (newthread < 0) {
		thread = 0;
        return -newthread;
	}

	struct Tcb *t = &env->env_threads[newthread];
	t->tcb_tf.regs[29] = USTACKTOP - 4 * BY2PG * newthread - 4;
	t->tcb_tf.pc = start_routine;
	t->tcb_tf.regs[4] = arg;
	t->tcb_tf.regs[31] = exit;
	syscall_set_thread_status(t->tcb_id, ENV_RUNNABLE);
	*thread = t->tcb_id;
	return 0;
}

void pthread_exit(void *retval) {
	u_int tcbid = syscall_get_tcbid();
	struct Tcb *t = &env->env_threads[tcbid & 0x7];
	t->tcb_exit_ptr = retval;
	syscall_thread_destroy(tcbid);
}

int pthread_cancel(pthread_t thread) {
	struct Tcb *t = &env->env_threads[thread & 0x7];

	if ((t->tcb_id != thread) || (t->tcb_status == ENV_FREE)) {
		return -E_THREAD_NOT_FOUND;
	}

	if (t->tcb_cancel_state == THREAD_CANNOT_CANCEL) {
		return -E_THREAD_CANNOT_CANCEL;
	}

	t->tcb_exit_value = -THREAD_CANCEL_EXIT;
	if (t->tcb_cancel_type == THREAD_CANCEL_ASYN) {
		syscall_thread_destroy(thread);
	} else {
		t->tcb_canceled = 1;
	}

	return 0;
}

int pthread_setcancelstate(int state, int *oldstate) {
	u_int tcbid = syscall_get_tcbid();
	struct Tcb *t = &env->env_threads[tcbid & 0x7];
	
	if ((state != THREAD_CAN_CANCEL) && (state != THREAD_CANNOT_CANCEL)) {
		return -E_INVAL;
	}

	if (t->tcb_id != tcbid) {
		return -E_INVAL;
	}

	if (oldstate != 0) {
		*oldstate = t->tcb_cancel_state;
	}

	t->tcb_cancel_state = state;
	return 0;
}

int pthread_setcanceltype(int type, int *oldtype) {
	u_int tcbid = syscall_get_tcbid();
	struct Tcb *t = &env->env_threads[tcbid & 0x7];
	
	if ((type != THREAD_CANCEL_ASYN) && (type != THREAD_CANCEL_DEFER)) {
		return -E_INVAL;
	}

	if (t->tcb_id != tcbid) {
		return -E_INVAL;
	}

	if (oldtype != 0) {
		*oldtype = t->tcb_cancel_type;
	}

	t->tcb_cancel_type = type;
	return 0;
}

void pthread_testcancel() {
	u_int tcbid = syscall_get_tcbid();
	struct Tcb *t = &env->env_threads[tcbid & 0x7];

	if (t->tcb_id != tcbid) {
		user_panic("something wrong in pthread_testcancel!\n");
	}

	if ((t->tcb_canceled) && 
		(t->tcb_cancel_state == THREAD_CAN_CANCEL) && 
		(t->tcb_cancel_type == THREAD_CANCEL_DEFER)) {
		t->tcb_exit_value = -THREAD_CANCEL_EXIT;
		syscall_thread_destroy(t->tcb_id);
	}
}

int pthread_detach(pthread_t thread) {
	struct Tcb *t = &env->env_threads[thread & 0x7];
	int r, i;
	
	if (t->tcb_id != thread) {
		return -E_THREAD_NOT_FOUND;
	}

	if (t->tcb_status == ENV_FREE) {
		u_int sp = USTACKTOP - BY2PG * 4 * (thread & 0x7);
		for(i = 1; i <= 4; ++i) {
			r = syscall_mem_unmap(0, sp - i * BY2PG);
			if (r < 0)
				return r;
		}
		user_bzero(t, sizeof(struct Tcb));
	} else {
		t->tcb_detach = 1;
	}
	return 0;
}

int pthread_join(pthread_t thread, void **retval) {
	int r = syscall_thread_join(thread, retval);
	return r;
}
