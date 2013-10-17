#include "syscall.h"
#include "task.h"
#include "string.h"

#define MAX_COMMAND_LEN 30

extern struct task_control_block tasks[];

enum {
	PS = 0,
	HELP,
	HELLO,
	SYSTEM,
	MAX_COMMANDS
};

typedef struct{
	char* name;
	void (*function)(void);
} shell_cmd;

void print(char * print_str){
	int fdout;
	fdout= mq_open("/tmp/mqueue/out", 0);
	write(fdout, print_str, strlen(print_str)+1);
	if(print_str[strlen(print_str)-1] == '\n')
	{
		write(fdout, "\r", 2);
	}
}

void read_string(char *string){
	int fdin;
	char ch[2] = {0};
	int curr_char = 0;
	int done = 0;
	
	fdin = open("/dev/tty0/in", 0);
	do{
		//GDBLABEL:READ
		read(fdin, ch, 1);
		if(curr_char >= MAX_COMMAND_LEN || (ch[0] == '\r') || (ch[0] == '\n')){
			//GDBLABEL:SKIP_DONE
			string[curr_char] = '\0';
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
void ps_func(){
	int i;
	char num_char[5];
	print("PID\tStatus\tPriority\n");
	for(i = 0; i < TASK_LIMIT; i++){
		print(itoa(num_char, tasks[i].pid));
		print("\t");
		task_info(tasks[i].status);
		print("\t");
		print(itoa(num_char, tasks[i].priority));
		print("\n");
	}
}

void help_func(){
	print("What can I help you?\n");
}

void hello_func(){
	print("Hello World\n");
}

void system_func(){
	char command[MAX_COMMAND_LEN];
	print("Please enter your command:");
	read_string(command);
	union sys_param func_param[2] = {
		{.pCHAR = command},
		{.pINT = strlen(command)}
	};
	host_call(0x12, func_param);
}

shell_cmd commands[] = {
	{
		.name = "ps",
		.function = ps_func
	},
	{
		.name = "help",
		.function = help_func
	},
	{
		.name = "hello",
		.function = hello_func
	},
	{
		.name = "system",
		.function = system_func
	}
};

void serial_shell_task(){
	char command[MAX_COMMAND_LEN];
	int i;

	while(1){
		print("evshary->");
		read_string(command);

		for(i = 0; i < MAX_COMMANDS;i++){
			if(!strcmp(commands[i].name, command)){
				commands[i].function();
				break;
			}
		}
		
	}
}
