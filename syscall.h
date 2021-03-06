#ifndef SYSCALL_H
#define SYSCALL_H
#include <stddef.h>

union sys_param{
	int pINT;
	char *pCHAR;
};

void *activate(void *stack);

int fork();
int getpid();

int write(int fd, const void *buf, size_t count);
int read(int fd, void *buf, size_t count);

void interrupt_wait(int intr);

int getpriority(int who);
int setpriority(int who, int value);

int mknod(int fd, int mode, int dev);

void sleep(unsigned int);

int host_call(int action, void *arg);
#endif
