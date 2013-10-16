#include "syscall.h"
#include "path.h"
#include "task.h"
#include "string.h"

void rs232_xmit_msg_task()
{
	int fdout, fdin;
	char str[100];
	int curr_char;
	fdout = open("/dev/tty0/out", 0);
	fdin = mq_open("/tmp/mqueue/out", O_CREAT);
	setpriority(0, PRIORITY_DEFAULT - 2);

	while (1) {
		/* Read from the queue.  Keep trying until a message is
		 * received.  This will block for a period of time (specified
		 * by portMAX_DELAY). */
		read(fdin, str, 100);

		/* Write each character of the message to the RS232 port. */
		curr_char = 0;
		while (str[curr_char] != '\0') {
			write(fdout, &str[curr_char], 1);
			curr_char++;
		}
	}
}

void queue_str_task(const char *str, int delay)
{
	int fdout = mq_open("/tmp/mqueue/out", 0);
	int msg_len = strlen(str) + 1;

	while (1) {
		/* Post the message.  Keep on trying until it is successful. */
		write(fdout, str, msg_len);

		/* Wait. */
		sleep(delay);
	}
}

void queue_str_task1()
{
	queue_str_task("Hello 1\n", 200);
}

void queue_str_task2()
{
	queue_str_task("Hello 2\n", 50);
}

void serial_readwrite_task()
{
	int fdout, fdin;
	char str[100];
	char ch;
	int curr_char;
	int done;

	fdout = mq_open("/tmp/mqueue/out", 0);
	fdin = open("/dev/tty0/in", 0);

	/* Prepare the response message to be queued. */
	memcpy(str, "Got:", 4);

	while (1) {
		curr_char = 4;
		done = 0;
		do {
			/* Receive a byte from the RS232 port (this call will
			 * block). */
			read(fdin, &ch, 1);

			/* If the byte is an end-of-line type character, then
			 * finish the string and inidcate we are done.
			 */
			if (curr_char >= 98 || (ch == '\r') || (ch == '\n')) {
				str[curr_char] = '\n';
				str[curr_char+1] = '\0';
				done = -1;
				/* Otherwise, add the character to the
				 * response string. */
			}
			else {
				str[curr_char++] = ch;
			}
		} while (!done);

		/* Once we are done building the response string, queue the
		 * response to be sent to the RS232 port.
		 */
		write(fdout, str, curr_char+1+1);
	}
}

unsigned int *init_task(unsigned int *stack, void (*start)())
{
	stack += STACK_SIZE - 9; /* End of stack, minus what we're about to push */
	stack[8] = (unsigned int)start;
	return stack;
}

int
task_push (struct task_control_block **list, struct task_control_block *item)
{
	if (list && item) {
		/* Remove itself from original list */
		if (item->prev)
			*(item->prev) = item->next;
		if (item->next)
			item->next->prev = item->prev;
		/* Insert into new list */
		while (*list) list = &((*list)->next);
		*list = item;
		item->prev = list;
		item->next = NULL;
		return 0;
	}
	return -1;
}

struct task_control_block*
task_pop (struct task_control_block **list)
{
	if (list) {
		struct task_control_block *item = *list;
		if (item) {
			*list = item->next;
			if (item->next)
				item->next->prev = list;
			item->prev = NULL;
			item->next = NULL;
			return item;
		}
	}
	return NULL;
}
