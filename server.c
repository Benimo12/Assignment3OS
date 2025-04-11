/*********************************************************************/
/*                                                                   */
/* server.c                                                          */
/*                                                                   */
/* description:  This server program creates a message descriptor    */
/*               using a well-know SERVICEKEY. With this message     */
/*               it receives messages from clients. Once it receives */
/*               a message, a child process is created to handle it. */
/*               The child process will then send a number puzzle,   */
/*               and wait till an answer it received. The child      */
/*               then responds with another message.                 */
/*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*********************************************************************/
/* The following hw3.h headfile defines the well known SERVICEKEY    */
/* and the message structure.                                        */
/*********************************************************************/
#include "hw3.h"

/*
 * In this program, the server will first send a randomly picked number
 * puzzle to the client. It waits for the answer, and then respond with
 * another message to the client accordingly.
 */

static void solve_a_puzzle(int clientpid, int msgid)
{
	struct MESSAGE_STRUCT   msg;
	int puzzle;
	int answer;
	int response;
	FILE *fp;
	int size;
	char filename[100];

	/*
	 * Randomly pick a puzzle file, and read the text into the message body
	 */
	srand48(getpid());
	puzzle = lrand48() % 5;
	sprintf(filename, "PUZZLE-%d", puzzle);
	fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(msg.mtext, 1, size, fp);
	msg.mtext[size] = 0;
	fclose(fp);
	/*
	 * Set mtype to clientpid, indicating it is for that client process
	 */
	msg.mtype = clientpid;

	/*
	 * Send the message to that client process
	 */
	if (msgsnd(msgid, &msg, 1000, 0) == -1) {
		perror("msgsnd()");
		exit(4);
	}

	/*
	 * Afterwards, recieve the answer from the client process.
	 * the client process should set mtype to be 0x7fffffff-clientpid.
	 * Note, clientpid is used by the outgoing messages from the server, and
	 * we have to use another value for mtype in the incoming messages.
	 * Here, 0x7fffffff is the maximum 32-bit integer value. This method 
	 * usually works well.
	 */

	if (msgrcv(msgid, &msg, 1000, 0x7fffffff-clientpid, 0) == -1) {
		perror("msgsnd()");
		exit(5);
	}

	/*
	 * The client process's response is the first 4-byte integer
	 * in the message body
	 */
	answer = *(int*)msg.mtext;
//	answer = atoi(msg.mtext);
	
	/*
	 * Check if the answer matches the correct one, and pick a response
	 * accordingly.
	 */
	if (answer == puzzle)
		response = lrand48() % 4;
	else
		response = 4 + lrand48() % 2;
	/*
	 * Now read the text into the response message body
	 */
	sprintf(filename, "RESPONSE-%d", response);
	fp = fopen(filename, "r");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	fread(msg.mtext, 1, size, fp);
	msg.mtext[size] = 0;
	fclose(fp);

	/*
	 * Set mtype to clientpid, indicating it is for that client process
	 */
	msg.mtype = clientpid;

	/*
	 * Finally, send the message
	 */
	if (msgsnd(msgid, &msg, 1000, 0) == -1) {
		perror("msgsnd()");
		exit(4);
	}
}

/*
 * This is the main program. It is in a infinite loop. Whenever it receives
 * the first message from a client, a child process is created to handle the
 * message interaction with the client.
 */
main()
{
	int			msgid;
	struct MESSAGE_STRUCT	msg;
	int			clientpid, childpid;

	/*
	 * Create a message descriptor with the service key. Return an
	 * error if the descriptor already exists. Permissions
	 * are given as everyone can send/receive messages.
	 */
	if ((msgid = msgget(SERVICEKEY, 0777|IPC_CREAT)) == -1) {
		perror("msgget()");
		exit(1);
	}
	msgctl(msgid, IPC_RMID, NULL );
	if ((msgid = msgget(SERVICEKEY, 0777|IPC_CREAT)) == -1) {
		perror("msgget()");
		exit(1);
	}
	printf("The server has started. The message queue ID is:%d\n",msgid);

	while(1) {
		/*
		 * the server will receive a message of type=1
		 */
		if (msgrcv(msgid, &msg, 1000, 1, 0) == -1) {
			perror("msgrcv()");
			exit(2);
		}
		/*
		 * Which client sends the message to me?
		 */
		clientpid = *(int*)msg.mtext;
		printf("the clientpid after casting is from within server is: %d ",clientpid);
		printf("The client ID is %d ",clientpid);
//		clientpid = msg.mtype;
		/*
		 * A child process is created to handle the message
		 */
		if ((childpid = fork()) < 0) {
			perror("fork()");
			exit(3);
		} else if (childpid == 0) {
			/*
			 * this is the program to handle all the message
			 * interaction with the client.
			 */
			solve_a_puzzle(clientpid, msgid);
			exit(0);
		}
	}
}
