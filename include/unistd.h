#ifndef UNISTD_H
#define UNISTD_H

#define __SYSCALL_BASE 9527
#define __NR_SYSCALLS 25


#define SYS_putchar 		((__SYSCALL_BASE ) + (0 ) ) 
#define SYS_getenvid 		((__SYSCALL_BASE ) + (1 ) )
#define SYS_yield			((__SYSCALL_BASE ) + (2 ) )
#define SYS_env_destroy		((__SYSCALL_BASE ) + (3 ) )
#define SYS_set_pgfault_handler	((__SYSCALL_BASE ) + (4 ) )
#define SYS_mem_alloc		((__SYSCALL_BASE ) + (5 ) )
#define SYS_mem_map			((__SYSCALL_BASE ) + (6 ) )
#define SYS_mem_unmap		((__SYSCALL_BASE ) + (7 ) )
#define SYS_env_alloc		((__SYSCALL_BASE ) + (8 ) )
#define SYS_set_env_status	((__SYSCALL_BASE ) + (9 ) )
#define SYS_set_trapframe		((__SYSCALL_BASE ) + (10 ) )
#define SYS_panic			((__SYSCALL_BASE ) + (11 ) )
#define SYS_ipc_can_send		((__SYSCALL_BASE ) + (12 ) )
#define SYS_ipc_recv		((__SYSCALL_BASE ) + (13 ) )
#define SYS_cgetc			((__SYSCALL_BASE ) + (14 ) )

#define SYS_get_tcbid       ((__SYSCALL_BASE ) + (15 ) )
#define SYS_thread_alloc    ((__SYSCALL_BASE ) + (16 ) )
#define SYS_thread_destroy  ((__SYSCALL_BASE ) + (17 ) )
#define SYS_set_thread_status   ((__SYSCALL_BASE ) + (18 ) )
#define SYS_thread_join     ((__SYSCALL_BASE ) + (19 ) )

#define SYS_sem_destroy     ((__SYSCALL_BASE ) + (20 ) )
#define SYS_sem_wait        ((__SYSCALL_BASE ) + (21 ) )
#define SYS_sem_trywait     ((__SYSCALL_BASE ) + (22 ) )
#define SYS_sem_post        ((__SYSCALL_BASE ) + (23 ) )
#define SYS_sem_getvalue    ((__SYSCALL_BASE ) + (24 ) )

#endif
