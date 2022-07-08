#include "lib.h"

void *pthreadtest(void *arg) {
	int arg1 = ((int *)arg)[0];
	int arg2 = ((int *)arg)[1];
	int arg3 = ((int *)arg)[2];
	int *stacki = (int *)((int *)arg)[3];

	writef("arg 1 is %d\n", arg1);
	writef("arg 2 is %d\n", arg2);
	writef("arg 3 is %d\n", arg3);
	if (arg1 == 1 && arg2 == 2 && arg3 == 3) {
		writef("passing args success!\n");
	} else {
		user_panic("passing args error!\n");
	}

	(*stacki) = 1;
	writef("[[%x]] stacki = %d\n", syscall_get_tcbid(), *stacki);
}

void umain() {
    writef("Test start ... \n");
	pthread_t thread;
	int tcbid;
	int i = 0;
	int args[4];
	args[0] = 1;
	args[1] = 2;
	args[2] = 3;
	args[3] = &i;

	tcbid = pthread_create(&thread, NULL, pthreadtest, (void *)args);
	if (tcbid == 0) {
		writef("thread create success!\n");
	} else {
		user_panic("thread create error!\n");
	}

	while(1) {
		writef("");
		if (i != 0) break;
		syscall_yield();
	}
	writef("[[%x]] i = %d\n", syscall_get_tcbid(), i);
}