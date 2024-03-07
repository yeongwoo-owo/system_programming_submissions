#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXUSER 20
#define MAXNAME 10
#define MAXLINE 80

int main (int argc, char *argv[]) {

    int n, listenfd, connfd, caddrlen, fdmax, fdnum, usernum=0;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    char name[MAXUSER][MAXNAME];
	for(int i=0;i<MAXUSER;i++){
		name[i][0]=0;
	}
    char temp[MAXLINE+MAXNAME+1];
    int port = atoi(argv[1]);
    fd_set readset, copyset;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    /* Insert your code for file descriptor set
	*
	*
	**/
	
	FD_ZERO(&readset);
	FD_SET(listenfd, &readset);
	fdmax=listenfd;
	n=read(listenfd, buf, MAXNAME);
	buf[n]='\0';
	strcpy(name[listenfd], buf);

    while (1) {

        copyset = readset;

        if((fdnum=select(fdmax+1, &copyset, NULL, NULL, NULL) < 0 )) {
            printf("select() failed.\n");
            exit(4);
        }

       for (int i = 3; i < fdmax + 1; i++) {

           memset(buf, 0, MAXLINE);
           memset(temp, 0, MAXLINE+MAXNAME+1);

            if (FD_ISSET(i, &copyset)) {

                if (i == listenfd) {
                    
                    caddrlen = sizeof(caddr);
                    if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0) {
                        printf ("accept() failed.\n");
                        continue;
                    }

                    /* Insert your code */ 
					FD_SET(connfd, &readset);
					if(fdmax<connfd){
						fdmax=connfd;
					}
					

                }
                else {

                    /* Insert your code */
					if((n=read(i, buf, MAXLINE))>0){
						if(!strcmp(buf, "quit\n")){
							FD_CLR(i, &readset);
							sprintf(temp, "%s leaved. %d current user.", name[i], --usernum);
							for(int j=4;j<fdmax+1;j++){
								write(j, temp, MAXLINE+MAXNAME+1);
							}
							close(i);
						}else{
							printf("got %d bytes from user %s\n", n, name[i]);
							for(int j=4;j<fdmax+1;j++){
								if(j!=i){
									write(j, buf, n);
								}
							}
						}
					}
                }
            }  
        }
    }

    return 0;

}

