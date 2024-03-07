#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void make_tokens(char* cmd, char* arg[], char* arg2[], char* target) {

   // insert your code
   // make tokens which will be used in pipe or redirections
   // you can change the parameters if you want

    int i=0;
    char* ptr=strtok(cmd, " ");

    while(ptr!=NULL&&strcmp(ptr, target)!=0){
		arg[i++]=ptr;
		ptr=strtok(NULL, " ");
	}
    arg[i]=NULL;

    i=0;
    ptr=strtok(NULL, " ");

	while(ptr!=NULL){
		arg2[i++]=ptr;
		ptr=strtok(NULL, " ");
	}
    arg2[i]=NULL;

    return;
}

int main () {

    size_t size;
    char *cmd;
    char *ptr;
    char *arg[128];
    char *arg2[128];
    char path[128];
    char path2[128];
    int child_status;
    int fd[2];
    int fdr;
    
    while(1) {

        int num = 0;
        cmd = NULL;
        getline(&cmd, &size, stdin);
        cmd[strlen(cmd)-1] = '\0';

        if(strcmp(cmd, "quit") == 0) {
            break;
        }

        if(strchr(cmd, '|') != NULL) {
            
            make_tokens(cmd, arg, arg2, "|");
            
            sprintf(path, "/bin/%s", arg[0]);
            sprintf(path2, "/bin/%s", arg2[0]);
            
            // insert your code
            // implement pipe using pipe and dup2 functions

            if (fork() == 0) {
                pipe(fd);

                if (fork() == 0) {
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    execv(path, arg);
                    exit(0);
                }else{
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    close(fd[1]);
                    wait(&child_status);
                    execv(path2, arg2);
                    exit(0);  
                }
            }else{
                wait(&child_status);  
            }
        } 
        else if(strchr(cmd, '>') != NULL) {

            make_tokens(cmd, arg, arg2, ">");
            sprintf(path, "/bin/%s", arg[0]);
            
            // insert your code
            // implement > redirection using dup2
            
            if (fork() == 0) {
                int fd=open(arg2[0], O_RDWR|O_CREAT|O_TRUNC, 0777);
                dup2(fd, STDOUT_FILENO);
                close(fd);

                execv(path, arg);
                exit(0);
            }
            else{
                wait(&child_status);  
            }  
        }
        else if(strchr(cmd, '<') != NULL) {

            make_tokens(cmd, arg, arg2, "<");
            sprintf(path, "/bin/%s", arg[0]);
            
            // insert your code
            // implement < redirection using dup2

            if (fork() == 0) {
                int fd=open(arg2[0], O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
                
                execv(path, arg);
                exit(0);
            }
            else{
                wait(&child_status);  
            }  
        }
        else {
        	
        	// mini shell from week5 - do not modify 
            ptr = strtok(cmd, " ");
            while (ptr != NULL) {
                arg[num++] = ptr;
                ptr = strtok(NULL, " ");
            }
            arg[num] = NULL;

            sprintf(path, "/bin/%s", arg[0]);
        
            if (fork() == 0) {
                execv(path, arg);
                exit(0);

            }
            else
                wait(&child_status);            

        }

    }

    return 0;

}
