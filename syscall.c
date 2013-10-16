#include <stddef.h>
#define SYSCALL_FORK		(0x01)
#define SYSCALL_GETPID		(0x02)
#define SYSCALL_WRITE		(0x03)
#define SYSCALL_READ		(0x04)
#define SYSCALL_WAIT_INTR	(0x05)
#define SYSCALL_GETPRIOR	(0x06)
#define SYSCALL_SETPRIOR	(0x07)
#define SYSCALL_MKNOD		(0x08)
#define SYSCALL_SLEEP		(0x09)

#define QUOTE(NUM) #NUM
#define TOCALL(NUM) QUOTE(NUM)

#define SYS_CALL_STRUCT(ACT) \
	__asm__( \
		"push {r7}\n" \
		"mov r7," ACT"\n" \
		"svc 0\n" \
		"nop\n" \
		"pop {r7}\n" \
		"bx lr\n" \
		:::\
	);

int fork() __attribute__ ((naked));
int fork(){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_FORK));
}

int getpid() __attribute__ ((naked));
int getpid(){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_GETPID));
}

int write(int fd, const void *buf, size_t count) __attribute__ ((naked));
int write(int fd, const void *buf, size_t count){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_WRITE));
}

int read(int fd, void *buf, size_t count) __attribute__ ((naked));
int read(int fd, void *buf, size_t count){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_READ));
}

void interrupt_wait(int intr) __attribute__ ((naked));
void interrupt_wait(int intr){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_WAIT_INTR));
}

int getpriority(int who) __attribute__ ((naked));
int getpriority(int who){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_GETPRIOR));

}

int setpriority(int who, int value) __attribute__ ((naked));
int setpriority(int who, int value){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_SETPRIOR));
}

int mknod(int fd, int mode, int dev) __attribute__ ((naked));
int mknod(int fd, int mode, int dev){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_MKNOD));
}

void sleep(unsigned int msec) __attribute__ ((naked));
void sleep(unsigned int msec){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_SLEEP));
}

int host_call(int action, void *arg) __attribute__((naked));
int host_call(int action, void *arg){
	__asm__(\
		"bkpt 0xAB\n"\
		"nop\n"\
		"bx lr\n"\
		:::\
	);
}
