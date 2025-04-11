
#define SERVICEKEY 2025		/* Key used in creating a message descriptor */

struct MESSAGE_STRUCT {
	long	mtype;		/* message type */
				/* the client should set mtype=1, for server */
				/* the server set mtype to the client's pid  */
	char	mtext[1000];	/* The first (4 bytes int) is client's pid,  */
				/* followed by the command "GET" or "POST",  */
				/* then followed by the joke.                */
				/* For POST, a joke follows the GET keyword; */
				/* for GET, the server will append a joke.   */
};

