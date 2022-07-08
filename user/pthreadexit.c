#include "lib.h"

void *pthreadexit(void *arg) {
	writef("pthreadexit start ...\n");
	writef("[[%x]] arg %d\n", syscall_get_tcbid(), *((int *)arg));
	pthread_exit(arg);
	user_panic("thread didn't exit!\n");
}

void umain() {
	writef("Test start ... \n");
	pthread_t thread;
	int tcbid;
	int arg = 99;
	tcbid = pthread_create(&thread, NULL, pthreadexit, (void *)(&arg));
	if (tcbid == 0) {
		writef("thread create success!\n");		
	} else {
		user_panic("thread create error!\n");
	}
	while (env->env_threads[thread & 0x7].tcb_status != ENV_FREE) {
		writef("");
	}
	writef("get retval %d, expecting %d\n", *((int *)env->env_threads[thread & 0x7].tcb_exit_ptr), arg);
	if (*((int *)env->env_threads[thread & 0x7].tcb_exit_ptr) == arg) {
		writef("retval correct!\n");
	} else {
		user_panic("retval incorrect!\n");
	}
}
