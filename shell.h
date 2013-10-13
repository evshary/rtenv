void greeting()
{
	int fdout = open("/dev/tty0/out", 0);
	char *string = "Hello, World!\n";
	while (*string) {
		write(fdout, string, 1);
		string++;
	}
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

void print(int fdout, char * print_str){
	write(fdout, print_str, strlen(print_str)+1);
}

void task_info(int fdout, int status){
	switch(status){
		case 0:
			print(fdout, "TASK_READY\0");
			break;
		case 1:
			print(fdout, "TASK_WAIT_READ\0");
			break;
		case 2:
			print(fdout, "TASK_WAIT_WRITE\0");
			break;
		case 3:
			print(fdout, "TASK_WAIT_INTR\0");
			break;
		case 4:
			print(fdout, "TASK_WAIT_TIME\0");
			break;
	}
}

void serial_shell_task(){
	int fdin, fdout;
	char str[1024];
	char echo_char[2];
	char ch;
	int curr_char;
	int done;

	fdout= mq_open("/tmp/mqueue/out", 0);
	fdin = open("/dev/tty0/in", 0);
	

	while(1){
		memcpy(str, "\revshary@evshary-rtenv:~$\0", 26);
		write(fdout, str, 26);
		//print(fdout, "\revshary@evshary-rtenv:~$\0");		

		curr_char = 0;
		done = 0;
		do{
			//GDBLABEL:READ
			read(fdin, &ch, 1);
			if(curr_char >= 1024 || (ch == '\r') || (ch == '\n')){
				//GDBLABEL:SKIP_DONE
				str[curr_char] = '\0';
				print(fdout, "\n\0");
				done = 1;
			}else if(ch == 0x7f){
				curr_char--;
				print(fdout, "\b \b\0");
			}else{
				str[curr_char++] = ch;
				echo_char[0] = ch;
				echo_char[1] = '\0';
				write(fdout, echo_char, 2);
			}
		}while(!done);
		//GDBLABEL:STOP_TEST
		if(strcmp(str, "help") == 0){
			//GDBLABEL:TEST1
			print(fdout, "\rWhat can I help you?\n\0");
		}else if(strcmp(str, "hello") == 0){
			//GDBLABEL:TEST2
			print(fdout, "\rHello World!\n\0");
		}else if(strcmp(str, "echo") == 0){
			//Not Complete
		}else if(strcmp(str, "ps") == 0){
			//GDBLABEL:TEST3
			int i;
			print(fdout, "\rPID\tStatus\tPriority\n\0");
			for(i = 0; i < TASK_LIMIT; i++){
				print(fdout, "\r\0");
				print(fdout, itoa(str, tasks[i].pid));
				print(fdout, "\t");
				task_info(fdout, tasks[i].status);
				print(fdout, "\t");
				print(fdout, itoa(str, tasks[i].priority));
				print(fdout, "\n\0");
			}
		}else{
			print(fdout, "\rThis is wrong command!\n\0");
		}
		
	}
}
