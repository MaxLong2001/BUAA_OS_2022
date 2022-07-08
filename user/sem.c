#include "lib.h"
#include <error.h>

int sem_init(sem_t *sem, int pshared, unsigned int value) {
    if (sem == 0) {
        return -E_SEM_ERROR;
    }

    sem->sem_head_index = 0;
    sem->sem_tail_index = 0;
    sem->sem_envid = env->env_id;
    sem->sem_name[0] = '\0';
    sem->sem_value = value;
    sem->sem_shared = pshared;
    sem->sem_status = SEM_VALID;
    sem->sem_wait_count = 0;
    int i;
    for (i = 0; i < 10; i++) {
        sem->sem_wait_list[i] = 0;
    }
    return 0;
}

int sem_destroy(sem_t *sem) {
    return syscall_sem_destroy(sem);
}

int sem_wait(sem_t *sem) {
    return syscall_sem_wait(sem);
}

int sem_trywait(sem_t *sem) {
    return syscall_sem_trywait(sem);
}

int sem_post(sem_t *sem) {
    return syscall_sem_post(sem);
}

int sem_getvalue(sem_t *sem, int *valp) {
    return syscall_sem_getvalue(sem, valp);
}
