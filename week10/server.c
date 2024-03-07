#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAXLINE     200000

int main(int argc, char** argv){
    int n, listenfd, connfd, caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port=atoi(argv[1]);

    if((listenfd=socket(AF_INET, SOCK_STREAM, 0))<0){
        printf("socket() failed\n");
        exit(1);
    }

    memset((char*)&saddr, 0, sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(port);

    if(bind(listenfd, (struct sockaddr*) &saddr, sizeof(saddr))<0){
        printf("bind() failed\n");
        exit(2);
    }

    if(listen(listenfd, 5)<0){
        printf("listen() failed\n");
        exit(3);
    }

    while(1){
        caddrlen=sizeof(caddr);
        if((connfd=accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen))<0){
            printf("accept() failed\n");
            continue;
        }

        while((n=read(connfd, buf, MAXLINE))>0){
            buf[n-1]='\0';
            if(!strcmp(buf, "quit")){
                close(connfd);
                exit(0);
            }

            strcat(buf, "_copy");
            int file_copy=open(buf, O_RDWR|O_CREAT|O_TRUNC, 0777);
            n=read(connfd, buf, MAXLINE);
            write(file_copy, buf, n);
            printf("got %d bytes from client.\n", n);
            close(file_copy);
        }

    }
}