



		

	/* Command Line Interface Program
	 *
	 * BASIC SKELETAL STRUCTURE:
	 *
	 * 1) Make a command line to accept a command and give an output to it.
	 */

#include <stdio.h>
#include <string.h>

void function_1(){
	printf("Function_1 Executed Successfully\n");
}
void print_home_screen(){
	printf("\n\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printf("\t@                                             @\n");
	printf("\t@             Transport Scheduler             @\n");
	printf("\t@                                             @\n");
	printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n");
}
int main(){
	print_home_screen();
	char command[20];
	while(1){
		printf(">>");
		scanf("%20s",command);
		if(strcmp(command,"exec_function_1")==0){
			function_1();
		}
		else if(strcmp(command,"exit")==0){
			printf("Program Terminated\n");
			break;
		}
		else{
			printf("Unknown Command: %s\n",command);
		}
	}
}
//End of program

