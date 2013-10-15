#define SYSCALL_FORK		(0x01)
#define SYSCALL_GETTID		(0x02)
#define SYSCALL_WRITE		(0x03)
#define SYSCALL_READ		(0x04)
#define SYSCALL_WAIT_INTR	(0x05)
#define SYSCALL_GETPRIOR	(0x06)
#define SYSCALL_SETPRIOR	(0x07)
#define SYSCALL_MKNOD		(0x08)
#define SYSCALL_SLEEP		(0x09)

#define TOCALL(NUM) #NUM

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

int GETPID() __attribute__ ((naked));
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
int interrupt_wait(int intr){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_WAIT_INTR));
}

int getpriority(int who) __attribute__ ((naked));
int getpriority(int who){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_GETPRIOR));

}

int setpriority(int who, int value) __attribute__ ((naked));
int setpriority(int who, int value){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_FORK_SETPRIOR));
}

int mknod(int fd, int mode, int dev) __attribute__ ((naked));
int mknod(int fd, int mode, int dev){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_MKNOD));
}

void sleep(unsigned int) __attribute__ ((naked));
void sleep(unsigned int){
	SYS_CALL_STRUCT(TOCALL(SYSCALL_SLEEP));
}
