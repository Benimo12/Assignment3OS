/*	Bard College at Simon's Rock
	CMPT 351: Introduction to Operating Systems (Spring 2025)
	Assignment # 3
	Instructed by: Sami ALSHALWI*/

	// This program is for cleaning the memory after each experment. It removes the message queue from the memory.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "hw2.h"
main(){
int msgid;

if ((msgid = msgget(SERVICEKEY, 0777|IPC_CREAT)) == -1) {
		perror("msgget()");
		exit(1);
	}
	msgctl(msgid, IPC_RMID, NULL );
}