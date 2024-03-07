#include <unistd.h>
#include "scenarioLib.h"

int getStdin(char* cmd){
    char c;
    int idx=0;
    while(read(STDIN_FILENO, &c, sizeof(char))>0){
        if(c=='\n'){
            cmd[idx]='\0';
            return idx;
        }
        cmd[idx++]=c; 
    }
    return -1;
}

int main(int argc, char* argv[]){
    while(TRUE){
		char cmd[100];
        int cmdLen=getStdin(cmd);
        
		execute(argv[1], cmd, cmdLen);
    }

    return 0;
}
