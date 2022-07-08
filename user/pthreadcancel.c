#include "lib.h"

void *test1(void *args) {
	int oldval;
	pthread_setcancelstate(1, &oldval);
	writef("[[%x]] oldstate %d\n",syscall_get_tcbid(), oldval);
	while(1);
}

void *test2(void *args) {
	int oldval;
	int tcbid = syscall_get_tcbid();
	pthread_setcanceltype(1, &oldval);
	writef("[[%x]] oldtype %d and newtype 1\n", tcbid, oldval);
	pthread_setcancelstate(1, &oldval);
	writef("[[%x]] oldstate %d and newstate 1\n", tcbid, oldval);
	while (1) {
		writef("[[%x]] try cancel ...\n", tcbid);
		pthread_testcancel();
		writef("cannot be canceled!\n");
		syscall_yield();
	}
}

void umain() {
	writef("Test start ... \n");
	int arg = 1, ret, tcbid;
	pthread_t thread1, thread2;

	tcbid = pthread_create(&thread1, NULL, test1, (void *)arg);
	if (tcbid == 0) {
		writef("thread create success!\n");		
	} else {
		user_panic("thread create error!\n");
	}
	syscall_yield();
	
	ret = pthread_cancel(thread1);
	if (ret == 0) {
		writef("cancel success!\n");
	} else {
		user_panic("cancel fail!\n");
	}

	tcbid = pthread_create(&thread2, NULL, test2, (void *)arg);
	if (tcbid == 0) {
		writef("thread create success!\n");		
	} else {
		user_panic("thread create error!\n");
	}

	while (pthread_cancel(thread2) < 0);
	writef("thread testcancel success!\n");
	syscall_yield();
}
