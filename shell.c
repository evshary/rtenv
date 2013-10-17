#include "task.h"
#include "string.h"

#define MAX_READ 1024

extern struct task_control_block tasks[];

void read_string(char *string){
	int fdin;
	char ch[2] = {0};
	int curr_char = 0;
	int done = 0;
	
	fdin = open("/dev/tty0/in", 0);
	do{
		//GDBLABEL:READ
		read(fdin, ch, 1);
		if(curr_char >= MAX_READ || (ch[0] == '\r') || (ch[0] == '\n')){
			//GDBLABEL:SKIP_DONE
			print("\n");
			done = 1;
		}else if(ch[0] == 0x7f){
			curr_char--;
			print("\b \b\0");
		}else{
			string[curr_char++] = ch[0];
			print(ch);
		}
	}while(!done);

}

void greeting()
{
	int fdout = open("/dev/tty0/out", 0);
	char *string = "Hello, World!\n";
	while (*string) {
		write(fdout, string, 1);
		string++;
	}
}

int getDigitNum(int num){
	int digit = 1;
	while(num > 9){
		num /= 10;
		digit++;
	}
	return digit;
}


char* itoa(char* int_ptr, int num){
	int digitNum = getDigitNum(num);
	int i;
	int_ptr[digitNum] = '\0';
	for(i = digitNum-1; i >= 0; i--, num/=10){
		int_ptr[i] = "0123456789"[num%10];
	}
	return int_ptr;
}

void print(char * print_str){
	int fdout;
	fdout= mq_open("/tmp/mqueue/out", 0);
	write(fdout, print_str, strlen(print_str)+1);
	if(print_str[strlen(print_str)-1] == '\n')
	{
		write(fdout, "\r", 2);
	}
}

void task_info(int status){
	switch(status){
		case 0:
			print("TASK_READY");
			break;
		case 1:
			print("TASK_WAIT_READ");
			break;
		case 2:
			print("TASK_WAIT_WRITE");
			break;
		case 3:
			print("TASK_WAIT_INTR");
			break;
		case 4:
			print("TASK_WAIT_TIME");
			break;
	}
}

void serial_shell_task(){
	char str[1024];

	while(1){
		print("evshary->");

		read_string(str);

		//GDBLABEL:STOP_TEST
		if(strcmp(str, "help") == 0){
			//GDBLABEL:TEST1
			print("What can I help you?\n");
		}else if(strcmp(str, "hello") == 0){
			//GDBLABEL:TEST2
			print("Hello World!\n");
		}else if(strcmp(str, "system") == 0){
			print("Please enter your command:");
		}else if(strcmp(str, "ps") == 0){
			//GDBLABEL:TEST3
			int i;
			print("PID\tStatus\tPriority\n");
			for(i = 0; i < TASK_LIMIT; i++){
				print(itoa(str, tasks[i].pid));
				print("\t");
				task_info(tasks[i].status);
				print("\t");
				print(itoa(str, tasks[i].priority));
				print("\n");
			}
		}else{
			print("This is wrong command!\n");
		}
		
	}
}
