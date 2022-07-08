#include "lib.h"

void *test1(void *args) {
	int father_tcbid = *((int *)args);
	int tcbid = syscall_get_tcbid();
	u_int *retval;
	writef("test1 start ...\n");
	if (pthread_join(father_tcbid, &retval) == 0) {
		writef("[[%x]] join success!\n", tcbid);
	} else {
		writef("[[%x]] join fail!\n", tcbid);
		pthread_exit(NULL);
	}
	writef("[[%x]] father %x exit with retval %d\n", tcbid, father_tcbid, *retval);
}

void *test2(void *args) {
	u_int father_tcbid = *((int *)args);
	int tcbid = syscall_get_tcbid();
	u_int *retval;
	writef("test2 start ...\n");
	if (pthread_join(father_tcbid, &retval) == 0) {
		writef("[[%x]] join success!\n", tcbid);
	} else {
		writef("[[%x]] join fail!\n", tcbid);
		pthread_exit(NULL);
	}
	writef("[[%x]] father %x exit with retval %d\n", tcbid, father_tcbid, *retval);
}

void umain() {
	writef("Test start ... \n");
	u_int arg = syscall_get_tcbid();
	int ret = 99;
	pthread_t thread1, thread2;

	pthread_create(&thread1, NULL, test1, (void *)(&arg));
	syscall_yield();

	pthread_detach(arg);
	pthread_create(&thread2, NULL, test2, (void *)(&arg));
	syscall_yield();
	syscall_yield();
	
	pthread_exit(&ret);
}
