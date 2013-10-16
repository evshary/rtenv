#ifndef SHELL_H
#define SHELL_H

void greeting();

int getDigitNum(int num);

char* itoa(char* int_ptr, int num);

void print(int fdout, char * print_str);

void task_info(int fdout, int status);

void serial_shell_task();

#endif
