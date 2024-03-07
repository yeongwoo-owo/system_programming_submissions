#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
	char* cmd;
	size_t size;

	char* arg[100];

	while(1){
		getline(&cmd, &size, stdin);
		cmd[strlen(cmd)-1]='\0';
		
		if(strcmp(cmd, "quit")==0){
			break;
		}

		pid_t pid=fork();
		int child_status;

		if(pid==0){
			int i=0;
			char* ptr=strtok(cmd, " ");

			while(ptr!=NULL){
				arg[i++]=ptr;
				ptr=strtok(NULL, " ");
			}
		
			arg[i]=NULL;

			for(int j=0;j<i;j++){
      			printf("%s\n", arg[i]);
    		}

			char path[100];
			sprintf(path, "/bin/%s", arg[0]);
			execv(path, arg);
		}else{
			waitpid(pid, &child_status, 0);
		}
	}

	return 0;
}
		
