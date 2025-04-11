/*	Bard College at Simon's Rock
	CMPT 351: Introduction to Operating Systems (Spring 2025)
	Assignment # 3
	Instructed by: Sami ALSHALWI*/

	//Notce: this program is the client and you need to finish it.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "hw2.h"

main() {
	int clientpid;		
	
	int answer;		
	struct MESSAGE_STRUCT   msg; 	
	int	msgid;	

	clientpid=getpid();	


if ((msgid = msgget(SERVICEKEY, 0777|IPC_CREAT)) == -1) { 
		perror("msgget()");
		exit(1);
	}


printf("The client has started. ProcessID is: %d and Message queueId is: %d\n",clientpid,msgid); 

sprintf(msg.mtext, "%d",clientpid );
//memcpy(&msg.mtext[0], &clientpid , sizeof(int));
printf("%d",*(int*)msg.mtext);



	
	
msg.mtype=1;
	
if (msgsnd(msgid, &msg, 1000, 0) == -1) { 
		perror("msgsnd()");
		
			exit(2);
	}


if (msgrcv(msgid, &msg, 1000, clientpid, 0) == -1) { 
			perror("msgrcv()");
			exit(3);
		}

	printf("%s\n", msg.mtext);

	printf("what is the answer? :");

	scanf("%d",&answer);
	
	sprintf(msg.mtext, "%d",answer );
	//memcpy(&msg.mtext[0], &answer , sizeof(int));

	msg.mtype=0x7fffffff-clientpid; 
	if (msgsnd(msgid, &msg, 1000, 0) == -1) {
		perror("msgsnd()");
		exit(4);
	}
	
if (msgrcv(msgid, &msg, 1000, clientpid, 0) == -1) {
			perror("msgrcv()");
			exit(5);
		}

	printf("%s\n", msg.mtext); 
}
