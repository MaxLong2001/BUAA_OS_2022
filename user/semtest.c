#include "lib.h"

void *test1(void *args) {
	sem_t *sem = (sem_t *)(((u_int *)args));
	int tcbid = syscall_get_tcbid();
	int i, ret, value;

	ret = sem_getvalue(sem, &value);
	if (ret == 0) {
		writef("[[%x]] get value %d\n", tcbid, value);
	} else {
		user_panic("[[%x]] get value fail!\n", tcbid);
	}

	for (i = 0; i < 2; i++) {
		ret = sem_wait(sem);
		if (ret == 0) {
			writef("[[%x]] sem_wait success!\n", tcbid);
		} else {
			writef("[[%x]] sem_wait fail!\n", tcbid);
		}
		syscall_yield();
	}
}

void *test2(void *args) {
	sem_t *sem = (sem_t *)(((u_int *)args));
	int tcbid = syscall_get_tcbid();
	int i, ret, value;

	ret = sem_getvalue(sem, &value);
	if (ret == 0) {
		writef("[[%x]] get value %d\n", tcbid, value);
	} else {
		user_panic("[[%x]] get value fail!\n", tcbid);
	}

	for (i = 0; i < 2; i++) {
		ret = sem_trywait(sem);
		if (ret == 0) {
			writef("[[%x]] sem_trywait success!\n", tcbid);
		} else {
			writef("[[%x]] sem_trywait fail!\n", tcbid);
		}
		syscall_yield();
	}
}

void umain() {
	writef("Test start ... \n");
	pthread_t thread1, thread2;
	sem_t sem;
	u_int arg;
	int ret, value;
	int tcbid = syscall_get_tcbid();

	sem_init(&sem, 0, 1);
	arg = &sem;
	pthread_create(&thread1, NULL, test1, (void *)(arg));
	pthread_create(&thread2, NULL, test2, (void *)(arg));
	
	ret = sem_getvalue(&sem, &value);
	if (ret == 0) {
		writef("[[%x]] get value %d\n", tcbid, value);
	} else {
		user_panic("[[%x]] get value fail!\n", tcbid);
	}

	int i = 0;
	for (i = 0; i < 5; ++i) {
		if (value == 0) {
			writef("[[%x]] sem_post!\n", tcbid);
			sem_post(&sem);
		} 
		syscall_yield();
		ret = sem_getvalue(&sem, &value);
		if (ret < 0) {
			user_panic("[[%x]] get value fail!\n", tcbid);
		}
	}

	sem_destroy(&sem);
	ret = sem_getvalue(&sem, &value);
	if (ret < 0) {
		writef("sem_destroy success!\n");
	} else {
		user_panic("sem_destroy fail!\n");
	}
}