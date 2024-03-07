//김영우 2017314333

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

extern int errno;
int status;

//error
void commandNotFoundError(){
	fprintf(stderr, "swsh: Command not found\n");
	exit(1);
}

void fileNotFoundError(){
	fprintf(stderr, "swsh: No such file\n");
	exit(1);
}

void sigchldHandler(int sig){
	if(WIFSTOPPED(status)){
		kill(0, SIGKILL);
	}
	exit(0);
}

void sigtstpHandler(int sig){
	kill(getppid(), SIGCHLD);
	signal(SIGTSTP, SIG_DFL);
	kill(getpid(), SIGTSTP);
}

//defined function
void head(char** args){
	int n=10;
	char fileName[200];
	if(!strcmp(args[1], "-n")){
		n=atoi(args[2]);
		strcpy(fileName, args[3]);
	}else{
		strcpy(fileName, args[1]);
	}

	FILE* fp=fopen(fileName, "r");
	char buf[200];
	for(int i=0;i<n;i++){
		if(feof(fp)){
			printf("\n");
			return;
		}
		fgets(buf, 200, fp);
		printf("%s", buf);
	}
	fclose(fp);
	exit(0);
}

void tail(char** args){
	int n=10;
	char fileName[200];
	if(!strcmp(args[1], "-n")){
		n=atoi(args[2]);
		strcpy(fileName, args[3]);
	}else{
		strcpy(fileName, args[1]);
	}

	FILE* fp=fopen(fileName, "r");
	char buf[200];
	fpos_t linePos[1000];
	int nLine=1;
	char c;

	fseek(fp, 0, SEEK_SET);
	fgetpos(fp, &linePos[1]);
	while(!feof(fp)){
		if((c=fgetc(fp))=='\n'){
			fgetpos(fp, &linePos[++nLine]);
		}
	}

	fsetpos(fp, &linePos[nLine-n>0?nLine-n+1:1]);
	while(!feof(fp)){
		fgets(buf, 200, fp);
		printf("%s", buf);
	}
	printf("\n");
	fclose(fp);
	exit(0);
}

void cat(char** args){
	FILE* fp=fopen(args[1], "r");
	char buf[200];
	while(!feof(fp)){
		fgets(buf, 200, fp);
		printf("%s", buf);
	}
	printf("\n");
	fclose(fp);
	exit(0);
}

void cp(char** args){
	FILE* s=fopen(args[1], "r");
	if(access(args[2], F_OK)==0){
		return;
	}
	FILE* d=fopen(args[2], "w");
	char buf[200];

	while(!feof(s)){
		fgets(buf, 200, s);
		fprintf(d, "%s", buf);
	}
	exit(0);
}

void mv(char** args){
	int ret=rename(args[1], args[2]);
	if(ret==-1){
		perror("mv");
	}
	exit(0);
}

void rm(char** args){
	int ret=unlink(args[1]);
	if(ret==-1){
		perror("rm");
	}
	exit(0);
}

void cd(char** args){
	int ret=chdir(args[1]);
	if(ret==-1){
		perror("cd");
	}
	exit(0);
}

void pwd(){
	char cwd[200];
	getcwd(cwd, 200);
	printf("%s\n", cwd);
	exit(0);
}

void myExit(char** args){
	fprintf(stderr, "exit\n");

	int n=0;
	int digit=1;
	if(args[1]!=NULL){
		for(int i=strlen(args[1])-1;i>=0;i--){
			n+=(args[1][i]-'0')*digit;
			digit*=10;
		}
	}
	exit(n);
}

int getCmd(char* input, char cmd[200][200]){
	int nCmd=0, idx=0;
	for(int i=0;i<strlen(input);i++){
		if(input[i]=='|'){
			cmd[nCmd++][idx-1]='\0';
			idx=0;
			i++;
		}else{
			cmd[nCmd][idx++]=input[i];
		}
	}
	cmd[nCmd++][idx-1]='\0';

	return nCmd;
}

void redirection(int code, char* input, char* output){
	if(code==1){
		int fp=open(input, O_RDWR, 0777);
		if(fp==-1){
			fileNotFoundError();
			exit(1);
		}
		dup2(fp, STDIN_FILENO);
		close(fp);
	}else if(code==2){
		int fp=open(output, O_RDWR|O_CREAT|O_TRUNC, 0777);
		dup2(fp, STDOUT_FILENO);
		close(fp);
	}else if(code==3){
		int fp=open(output, O_RDWR|O_CREAT|O_APPEND, 0777);
		dup2(fp, STDOUT_FILENO);
		close(fp);
	}else{
		int fp1=open(input, O_RDWR, 0777);
		if(fp1==-1){
			fileNotFoundError();
			exit(1);
		}
		dup2(fp1, STDIN_FILENO);
		close(fp1);

		int fp2=open(output, O_RDWR|O_CREAT|O_TRUNC, 0777);
		dup2(fp2, STDOUT_FILENO);
		close(fp2);
	}
}

void execute(char* args[200]){
	char* cmd=args[0];
	char path[100];
	if(cmd[0]=='.'&&cmd[1]=='/'){
		system(cmd);
		exit(0);
	}else if(!strcmp(cmd, "ls")){
		sprintf(path, "/bin/%s", cmd);
		execv(path, args);
	}else if(!strcmp(cmd, "ls")){
		sprintf(path, "/bin/%s", cmd);
		execv(path, args);
	}else if(!strcmp(cmd, "man")||!strcmp(cmd, "grep")||!strcmp(cmd, "sort")||!strcmp(cmd, "awk")||!strcmp(cmd, "bc")){
		sprintf(path, "/usr/bin/%s", cmd);
		execv(path, args);
	}else if(!strcmp(cmd, "head")){
		head(args);
	}else if(!strcmp(cmd, "tail")){
		tail(args);
	}else if(!strcmp(cmd, "cat")){
		cat(args);
	}else if(!strcmp(cmd, "cp")){
		cp(args);
	}else if(!strcmp(cmd, "mv")){
		mv(args);
	}else if(!strcmp(cmd, "rm")){
		rm(args);
	}else if(!strcmp(cmd, "cd")){
		cd(args);
	}else if(!strcmp(cmd, "pwd")){
		pwd();
	}else if(!strcmp(cmd, "exit")){
		myExit(args);
	}else{
		commandNotFoundError();
	}
}

void run(char cmd[200][200], int n, int flag){
	char* args[200];
	char* inputDir=NULL;
	char* outputDir=NULL;
	int redirCode=0;
	int i=0;
	char* ptr=strtok(cmd[n], " ");
	
	while(ptr!=NULL){
		if(flag==1){
			if(!strcmp(ptr, "<")){
				redirCode=1;
				ptr=strtok(NULL, " ");
				inputDir=ptr;

				ptr=strtok(NULL, " ");
				if(ptr==NULL){
					break;
				}

				if(!strcmp(ptr, ">")){
					redirCode=4;
					ptr=strtok(NULL, " ");
					outputDir=ptr;
				}
				break;
			}else if(!strcmp(ptr, ">")){
				redirCode=2;
				ptr=strtok(NULL, " ");
				outputDir=ptr;
				break;
			}else if(!strcmp(ptr, ">>")){
				redirCode=3;
				ptr=strtok(NULL, " ");
				outputDir=ptr;
				break;
			}
		}else{
			if(!strcmp(ptr, "<")||!strcmp(ptr, ">")||!strcmp(ptr, ">>")){
				break;
			}
		}

		args[i++]=ptr;
		ptr=strtok(NULL, " ");
	}
	args[i]=NULL;

	if(flag==1&&redirCode!=0){
		redirection(redirCode, inputDir, outputDir);
	}

	execute(args);
}

void makeShell(char cmd[200][200], int fd[10][2], int n, int nCmd){			//cmd: 3개 -> fork 2번, pipe: 2개
	if(n==0){
		dup2(fd[n][1], STDOUT_FILENO);
		close(fd[n][0]);
		close(fd[n][1]);

		run(cmd, n, 1);
		exit(0);
	}else if(n==nCmd){
		if(fork()==0){
			makeShell(cmd, fd, n-1, nCmd);
		}else{
			waitpid(-1, &status, WNOHANG|WUNTRACED);
			// wait(NULL);
			dup2(fd[n-1][0], STDIN_FILENO);
			close(fd[n-1][0]);
			close(fd[n-1][1]);

			run(cmd, n, 1);
			exit(0);
		}
	}else{
		if(fork()==0){
			makeShell(cmd, fd, n-1, nCmd);
		}else{
			waitpid(-1, &status, WNOHANG|WUNTRACED);
			// wait(NULL);
			dup2(fd[n][1], STDOUT_FILENO);
			dup2(fd[n-1][0], STDIN_FILENO);
			// close(fd[n][0]);
			close(fd[n][1]);
			close(fd[n-1][0]);
			// close(fd[n-1][1]);

			run(cmd, n, 0);
			exit(0);
		}
	}
	
}

int main(void){
	// signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, sigtstpHandler);

	while(1){
		if(fork()==0){
			char input[200];
			fgets(input, 200, stdin);
			setpgid(0, 0);

			char cmd[200][200];
			int nCmd=getCmd(input, cmd);

			int fd[100][2];
			for(int i=0;i<nCmd-1;i++){
				pipe(fd[i]);
			}

			if(nCmd==1){
				run(cmd, 0, 1);
			}else{
				makeShell(cmd, fd, nCmd-1, nCmd-1);
			}
		}else{
			wait(NULL);
		}
	}
}
