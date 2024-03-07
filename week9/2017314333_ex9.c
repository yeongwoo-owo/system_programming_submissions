#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h> 


struct msg_norm
{
	long msgtype;
	/* implement here */
	char text1[256];
	char text2[256];
};


struct msg_ack 
{
	long msgtype;
	/* implement here */ 
	char timeStamp1[256];
	char timeStamp2[256];
};


int main()
{
	/* 
	 * @ int uid: user id 
	 * @ int receiver_id: receveri's id 
	 * */ 
	int uid; 
	int receiver_id; 
	printf("my id is\n");
	scanf("%d", &uid);
	getchar(); // flush   
	printf("who to send?\n");
	scanf("%d", &receiver_id); 
	getchar(); // flush  

	time_t curTime;
	time(&curTime);
		
	/* 
	 * code to get key and QID 
	 *
	 *
	 * */ 

	key_t key=ftok(".", 'a');
	int qid=msgget(key, IPC_CREAT|IPC_NOWAIT|0660);

	struct msg_norm* nMsg=malloc(sizeof(struct msg_norm));
	struct msg_ack* rMsg=malloc(sizeof(struct msg_ack));
	
	if (fork() == 0) 
	{
		while (1)
		{
			/* receive message  */ 
			/* get msg from queue and print out */ 
			if(uid<receiver_id){
				if(msgrcv(qid, nMsg, sizeof(struct msg_norm), 2, IPC_NOWAIT)!=-1){
					printf("RECEIVE %s\n", nMsg->text2);
					strcpy(rMsg->timeStamp2, ctime(&curTime));
					rMsg->msgtype=1;
					msgsnd(qid, rMsg, sizeof(struct msg_ack), 0);
				}

				if(msgrcv(qid, rMsg, sizeof(struct msg_ack), 2, IPC_NOWAIT)!=-1){
					printf("%d read message at:\t%s\n", receiver_id, rMsg->timeStamp2);
				}
			}else{
				if(msgrcv(qid, nMsg, sizeof(struct msg_norm), 1, IPC_NOWAIT)!=-1){
					printf("RECEIVE %s\n", nMsg->text1);
					strcpy(rMsg->timeStamp1, ctime(&curTime));
					rMsg->msgtype=2;
					msgsnd(qid, rMsg, sizeof(struct msg_ack), 0);
				}

				if(msgrcv(qid, rMsg, sizeof(struct msg_ack), 1, IPC_NOWAIT)!=-1){
					printf("%d read message at:\t%s\n", receiver_id, rMsg->timeStamp1);
				}
			}

		free(nMsg);
		free(rMsg);		
		}
	
	}
	else
	{
		while (1) 
		{
			/*0send message  */ 
			/* get user input and send to the msg queue*/
			if(uid>receiver_id){
				nMsg->msgtype=1;
				gets(nMsg->text1);
				msgsnd(qid, nMsg, sizeof(struct msg_norm), 0);
			}else{
				nMsg->msgtype=2;
				gets(nMsg->text2);
				msgsnd(qid, nMsg, sizeof(struct msg_norm), 0);
			}
			
		}

		free(nMsg);
		free(rMsg);	
		
	}	


	return 0;
}
