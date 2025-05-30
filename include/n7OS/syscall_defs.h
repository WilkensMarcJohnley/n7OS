#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#define NB_SYSCALL 5

int sys_example();

int sys_shutdown();

int sys_write();

int sys_fork();

int sys_exit();

typedef int (*fn_ptr)();
extern fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function);

#endif
