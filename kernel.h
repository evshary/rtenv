#ifndef KERNEL_H
#define KERNEL_H
#define STACK_SIZE 512 /* Size of task stacks in words */
#define TASK_LIMIT 8  /* Max number of tasks we can handle */
#define PIPE_BUF   64 /* Size of largest atomic pipe message */
#define PATH_MAX   32 /* Longest absolute path */
#define PIPE_LIMIT (TASK_LIMIT * 2)

#define PATHSERVER_FD (TASK_LIMIT + 3) 
	/* File descriptor of pipe to pathserver */

#define PRIORITY_DEFAULT 20
#define PRIORITY_LIMIT (PRIORITY_DEFAULT * 2 - 1)

#define TASK_READY      0
#define TASK_WAIT_READ  1
#define TASK_WAIT_WRITE 2
#define TASK_WAIT_INTR  3
#define TASK_WAIT_TIME  4

#define S_IFIFO 1
#define S_IMSGQ 2

#define O_CREAT 4

//shell function
void *memcpy(void *dest, const void *src, size_t n);
char* itoa(char* int_ptr, int num);
void print(int fdout, char * print_str);
void task_info(int fdout, int status);
void greeting();
void serial_shell_task();

int strcmp(const char *a, const char *b) __attribute__ ((naked));
int strcmp(const char *a, const char *b)
{
	asm(
       "strcmp_lop:                \n"
       "   ldrb    r2, [r0],#1     \n"
       "   ldrb    r3, [r1],#1     \n"
       "   cmp     r2, #1          \n"
       "   it      hi              \n"
       "   cmphi   r2, r3          \n"
       "   beq     strcmp_lop      \n"
		"	sub     r0, r2, r3  	\n"
       "   bx      lr              \n"
		:::
	);
}

size_t strlen(const char *s) __attribute__ ((naked));
size_t strlen(const char *s)
{
	asm(
		"	sub  r3, r0, #1			\n"
       "strlen_loop:               \n"
		"	ldrb r2, [r3, #1]!		\n"
		"	cmp  r2, #0				\n"
       "   bne  strlen_loop        \n"
		"	sub  r0, r3, r0			\n"
		"	bx   lr					\n"
		:::
	);
}

/* Stack struct of user thread, see "Exception entry and return" */
struct user_thread_stack {
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int fp;
	unsigned int _lr;	/* Back to system calls or return exception */
	unsigned int _r7;	/* Backup from isr */
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int ip;
	unsigned int lr;	/* Back to user thread code */
	unsigned int pc;
	unsigned int xpsr;
	unsigned int stack[STACK_SIZE - 18];
};

/* Task Control Block */
struct task_control_block {
    struct user_thread_stack *stack;
    int pid;
    int status;
    int priority;
    struct task_control_block **prev;
    struct task_control_block  *next;
};

struct task_control_block tasks[TASK_LIMIT];
#endif
