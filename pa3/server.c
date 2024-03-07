/*
    할 일
    1. 소켓 연결 확인
    2. client마다 쓰레드로 만들기
    3. mutex lock
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENT  1024
#define MAX_SEAT    256
#define MAX_USER    1024

int nClient;
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

struct query{
    int user;
    int action;
    int data;
};

struct userInfo{
    int pw;                     //초기값: -1
    int connectClient;          //연결된 클라이언트가 없으면 0, 있으면 fd값
    int reservedSeat;
};

struct userInfo userInfoArr[MAX_USER];
int seatArr[MAX_SEAT];

void handleQuery(int fd, struct query temp){
    int returnValue=-1;

    if(temp.user==0&&temp.action==0&&temp.data==0){
        send(fd, (int*)seatArr, sizeof(int)*MAX_SEAT, 0);
        close(fd);
        // nClient--;
        // if(nClient==0){
        //     exit(0);
        // }
        pthread_exit(NULL);
        return;
    }

    if(temp.user<0||temp.user>1023||temp.action<1||temp.action>5){
        send(fd, (int*)&returnValue, sizeof(int), 0);
        return;
    }

    switch(temp.action){
    case 1:         //log in
        // printf("log in\n");
        if(userInfoArr[temp.user].connectClient==0){
            if(userInfoArr[temp.user].pw==-1){     //첫 로그인
                pthread_mutex_lock(&m);
                userInfoArr[temp.user].connectClient=fd;
                userInfoArr[temp.user].pw=temp.data;
                userInfoArr[temp.user].reservedSeat=-1;
                pthread_mutex_unlock(&m);
                returnValue=1;
            }else if(userInfoArr[temp.user].pw==temp.data){     //재 로그인
                pthread_mutex_lock(&m);
                userInfoArr[temp.user].connectClient=fd;
                pthread_mutex_unlock(&m);
                returnValue=1;
            }
        }
        break;
    
    case 2:         //reserve
        // printf("reserve\n");
        if(userInfoArr[temp.user].connectClient==fd&&seatArr[temp.data]==-1){
            pthread_mutex_lock(&m);
            seatArr[temp.data]=temp.user;
            userInfoArr[temp.user].reservedSeat=temp.data;
            pthread_mutex_unlock(&m);
            returnValue=temp.data;
        }
        break;

    case 3:         //check reservation
        // printf("check reservation\n");
        if(userInfoArr[temp.user].connectClient==fd&&userInfoArr[temp.user].reservedSeat!=-1){
            pthread_mutex_lock(&m);
            returnValue=userInfoArr[temp.user].reservedSeat;
            pthread_mutex_unlock(&m);
        }
        break;

    case 4:         //cancel reservation
        // printf("cancel reservation\n");
        if(userInfoArr[temp.user].connectClient==fd&&seatArr[temp.data]!=-1){
            pthread_mutex_lock(&m);
            seatArr[temp.data]=-1;
            userInfoArr[temp.user].reservedSeat=-1;
            pthread_mutex_unlock(&m);
            returnValue=temp.data;
        }
        break;

    case 5:
        // printf("log out\n");
        if(userInfoArr[temp.user].connectClient==fd){
            pthread_mutex_lock(&m);
            userInfoArr[temp.user].connectClient=0;
            pthread_mutex_unlock(&m);
            returnValue=1;
        }
        break;
    }
    send(fd, (int*)&returnValue, sizeof(int), 0);
    // printf("%d\n", returnValue);
}

void* thread(void* arg){
    while(1){
        int fd=(int)arg;
        struct query tempQuery;
        recv(fd, (struct query*)&tempQuery, sizeof(struct query), 0);
        handleQuery(fd, tempQuery);
    }
}

int main(int argc, char* argv[]){
    int listenfd, connfd, caddrlen;
    // struct hostent* h;
    struct sockaddr_in saddr, caddr;
    int port=atoi(argv[1]);

    for(int i=0;i<MAX_USER;i++){
        userInfoArr[i].pw=-1;
    }

    if((listenfd=socket(AF_INET, SOCK_STREAM, 0))<0){
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char*)&saddr, 0, sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(port);

    if(bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr))<0){
        printf("bind() failed.\n");
        exit(2);
    }

    if(listen(listenfd, MAX_CLIENT)<0){
        printf("listen() failed.\n");
        exit(3);
    }

    /////////////////////   Socket  ////////////////////////

    memset(seatArr, -1, sizeof(int)*MAX_SEAT);

    while(1){
        caddrlen=sizeof(caddr);
        if((connfd=accept(listenfd, (struct sockaddr*)&caddr, (socklen_t*)&caddrlen))<0){
            printf("accept() failed.\n");
            continue;
        }
        
        // nClient++;

        pthread_t threadID;

        if(pthread_create(&threadID, NULL, thread, (void*)connfd)){
            printf("pthread create error\n");
            exit(4);
        }
    }

    pthread_mutex_destroy(&m);
}